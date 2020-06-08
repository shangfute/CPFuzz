'''
fuzz.py
----------
The 'main' file.
Imeplements both fuzz and fals algorithms and provides an option to
randomly simulate a system.

Please type
    ./fuzz.py --help
for usage details.

'''

from __future__ import print_function
import matplotlib.pyplot as plt
import logging
import numpy as np
import argparse
import pickle
import time
import sys as SYS
import os
import socket
import thread
import sysv_ipc
from signal import signal, SIGPIPE, SIG_DFL
import errno

import loadsystem
import traces

#precision=None, threshold=None, edgeitems=None, linewidth=None, suppress=True, nanstr=None, infstr=None, formatter=Nonu)
np.set_printoptions(suppress=True)


FORMAT = '[%(filename)s:%(lineno)s - %(funcName)20s() ] %(message)s'
FORMAT2 = '%(levelname) -10s %(asctime)s %(module)s:\
           %(lineno)s %(funcName)s() %(message)s'

serverAddr = 'cpfuzz.socket'

logging.basicConfig(filename='log.ff', filemode='w', format=FORMAT2,
                    level=logging.DEBUG)
logger = logging.getLogger(__name__)


def setup_shm():
# http://semanchuk.com/philip/sysv_ipc/#shared_memory
    try:
        memory = sysv_ipc.SharedMemory(sysv_ipc.IPC_PRIVATE, flags=sysv_ipc.IPC_CREAT|sysv_ipc.IPC_EXCL, mode=0o600, size=MAP_SIZE)
        if memory.id < 0:
            logger.info("shmget() failed")

        atexit(remove_shm);
        shm_str = str(memory.id)
        os.environ['SHM_ENV_VAR'] = str(memory.id)
        memory.attach()
        trace_bits = memory.read()
        return memory
        # memory.remove()

    except:
        logger.info("setup_shm failed")
        pass

def attach_shm():
    '''seems not work'''
    id_str = os.environ.get("SHM_ENV_VAR")
    if id_str != None:
        key = int(id_str)
        print(id)
        shm = ipc.SharedMemory(key, 0, 0)

        #I found if we do not attach ourselves
        #it will attach as ReadOnly.
        shm.attach(0,0)  
        buf = shm.read(19)
        print(buf)
        shm.detach()
        pass



def check_prop_violation(trace, prop):
    idx = prop.final_cons.contains(trace.x_array)
    return trace.x_array[idx], trace.t_array[idx]

def create_harness(sys,prop):
    harness = open('harness.c','r').read()
    num_dims = sys.num_dims

    # read(STDIN_FILENO, (int*)iv.int_state_arr, int_state_arr_num);
    stateAssign = []
    if num_dims.si > 0:
        stateAssign = ['iv.int_state_arr[{}]={};'.format(i,prop.initial_controller_state[i]) for i in range(num_dims.si)]
    harness = harness.replace('INT_STATE_ARR','\n'.join(stateAssign))

    # read(STDIN_FILENO, (double*)iv.float_state_arr, float_state_arr_num);
    stateAssign = []
    if num_dims.sf > 0:
        stateAssign = ['iv.float_state_arr[{}]={};'.format(i,prop.initial_controller_state[i]) for i in range(num_dims.si,num_dims.s)]
    harness = harness.replace('FLOAT_STATE_ARR','\n'.join(stateAssign))

    compareRob = ['rob = MAX(rob, MAX(iv.x_arr[{index}] -  {high}, ({low}) - iv.x_arr[{index}] ));'.format(high=prop.final_cons.h[i],low=prop.final_cons.l[i],index=i) for i in range(num_dims.x)]
    harness = harness.replace('COMPARE_ROB','\n'.join(compareRob))
    harness = harness.replace('inf','DBL_MAX')

    f = open(sys.path+'/harness.c','w')
    f.write(harness)
    f.close()
    os.system('cd '+sys.path+';make')



def create_corpus(sys,prop):
    num_segments = prop.num_segments      # sample time period
    num_dims = sys.num_dims
    init_cons = prop.init_cons

    f = open('seed_corpus/rand','wb')
    # read(STDIN_FILENO, (double*)iv.input_arr, input_arr_num);
    if num_dims.ci > 0 and (prop.ci.h - prop.ci.l > 0).any():
        ci_lb = prop.ci.l
        ci_ub = prop.ci.h
        ci_array = ci_lb + (ci_ub - ci_lb) * np.random.random((num_segments, num_dims.ci))  # URandom controller input
        f.write(ci_array.tobytes('C') )

    # read(STDIN_FILENO, (double*)iv.x_arr, x_arr_num);
    if num_dims.x > 0:
        x_array = init_cons.l + np.random.rand(init_cons.dim) * (init_cons.h - init_cons.l)     # random init state
        f.write(x_array.tobytes('C') )

    logger.info('URandom controller input  num_segments:{},num_dims.ci:{},num_dims.si:{},num_dims.sf:{},num_dims.x{}'.format( num_segments, num_dims.ci,num_dims.si,num_dims.sf,num_dims.x))
    f.close()

    # f = open('seed_corpus/min','wb')
    # # read(STDIN_FILENO, (double*)iv.input_arr, input_arr_num);
    # if num_dims.ci > 0 and (prop.ci.h - prop.ci.l > 0).any():
    #     for i in range(num_segments):
    #         f.write(prop.ci.l.tobytes('C') )

    # # read(STDIN_FILENO, (double*)iv.x_arr, x_arr_num);
    # if num_dims.x > 0:
    #     f.write(init_cons.l.tobytes('C') )
    # f.close()

    # f = open('seed_corpus/max','wb')
    # # read(STDIN_FILENO, (double*)iv.input_arr, input_arr_num);
    # if num_dims.ci > 0 and (prop.ci.h - prop.ci.l > 0).any():
    #     for i in range(num_segments):
    #         f.write(prop.ci.h.tobytes('C') )

    # # read(STDIN_FILENO, (double*)iv.x_arr, x_arr_num);
    # if num_dims.x > 0:
    #     f.write(init_cons.h.tobytes('C') )
    # f.close()

def setup_plant(sys,prop):
    os.system("rm *.socket")

    #create sockert
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    if sock < 0:
        print ('socket error')
    # bind to a file
    if os.path.exists(serverAddr):
        os.unlink(serverAddr)
    if sock.bind(serverAddr):
        print( 'socket.bind error')

    #listen
    if sock.listen(5):
        print ('socket.listen error')
    start_time = time.time()
    signal(SIGPIPE,SIG_DFL)

    while True:
        # logger.info( 'waiting for connecting')
        #waiting for client connecting
        conn, clientAddr = sock.accept()
        t = 0.0
        dummy_val = 0.0
        d = np.array( prop.initial_discrete_state)
        pvt = np.array(sys.plant_pvt_init_data)

        try:
            # receive plant init state n = write(sockfd, iv.x_arr, x_arr_num);   
            data = conn.recv(sys.num_dims.x  * 8)
            x0 = np.frombuffer(data, dtype=np.float64)
            x = x0
            # receive control output n = write(sockfd, rv.output_arr, output_arr_num);      
            data = conn.recv(sys.num_dims.u * 8)

            while data:
                uu = np.frombuffer(data, dtype=np.float64)

                (t, x, d, pvt) = sys.sim(
                        (t, t + sys.delta_t),
                        x,
                        d,
                        pvt,
                        uu,
                        dummy_val,
                        property_checker=None,
                        property_violated_flag=None
                        ) 
                # rob = min(rob,prop.final_cons.robustness(x))
                # if rob < 0:
                #     x_v = x
                # logger.info("t={},x={},u={}".format(t,x,uu))
                
                # send new state to controller n = read(sockfd, iv.x_arr, x_arr_num);    
                data = x.tobytes('C')
                conn.sendall(data)
                try:
                    data = conn.recv(sys.num_dims.u * 8)
                except socket.error as e:
                    if e.errno == errno.ECONNRESET:
                        break


            # # send robustness to controller n = read(sockfd, &result, sizeof(double));
            # conn.sendall(np.array(rob,dtype = np.float64).tobytes('C'))
            
        finally:
            # if rob<0:
            #     stop_time = time.time()
            #     logger.info('time spent(s) = {}, rob = {}, x0 = {} -> xv = {}'.format(stop_time - start_time, rob, x0, x_v))


            #close the connection
            conn.close()
        
    os.unlink(serverAddr)



def main():
    logger.info('execution begins')

    usage = '%(prog)s <filename>'
    parser = argparse.ArgumentParser(description='CPFuzz', usage=usage)
    parser.add_argument('-f','--filename', default=None, metavar='file_path.tst')
# uniform random fuzz
    parser.add_argument('-s', '--simulate', type=int, metavar='num-sims',
                        help='simulate')

# todo: fuzz using robust value
    parser.add_argument('-x', '--robust', type=int, metavar='num-sims',
                        help='using mtl robust value')

    parser.add_argument('-p', '--plot', action='store_true',
                        help='enable plotting')

    parser.add_argument('--dump', action='store_true',
                        help='dump trace in mat file')

    parser.add_argument('--seed', type=int, metavar='seed_value',
                        help='seed for the random generator')

    args = parser.parse_args()

    if args.filename is None:
        print('No file to test. Please use --help')
        exit()
    else:
        filepath = args.filename

    if args.seed is not None:
        np.random.seed(args.seed)

    Options = type('Options', (), {})
    opts = Options()
    opts.plot = args.plot

    sys, prop = loadsystem.parse(filepath)
    # if not os.path.exists(sys.path+"/fuzz-target"):
    create_harness(sys,prop)
    print("harness generated")
    create_corpus(sys,prop)

    # try:
    #     thread.start_new_thread( setup_plant, (sys,prop) )
    # except:
    #     print("Error: unable to start thread")  
    


    num_segments = prop.num_segments
    num_dims = sys.num_dims
    if (prop.ci.h - prop.ci.l > 0).any():
        cmd = 'afl-fuzz -P '+filepath+'  -m none -i seed_corpus -o out -- '+ sys.path +'/fuzz-target %d %d %d %d %d %d' % (num_segments,num_dims.ci, num_dims.si,num_dims.sf,num_dims.x,num_dims.u)
    else:
        cmd = 'afl-fuzz -P '+filepath+' -m none -i seed_corpus -o out -- '+ sys.path +'/fuzz-target %d %d %d %d %d %d' % (num_segments, 0 , num_dims.si,num_dims.sf,num_dims.x,num_dims.u)

    f = open('fuzz.sh','w')
    f.write('#!/bin/sh\n'+cmd)
    f.close()
    setup_plant(sys,prop)


if __name__ == '__main__':
    main()
    