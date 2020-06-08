#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>  
#include <stddef.h>  
#include <sys/socket.h>  
#include <sys/un.h>  
#include <errno.h>  
#include <float.h>

#include "controller.h"
#define MAXLINE 80  

#ifndef MIN
#  define MIN(_a,_b) ((_a) > (_b) ? (_b) : (_a))
#  define MAX(_a,_b) ((_a) > (_b) ? (_a) : (_b))
#endif /* !MIN */

char *client_path = "client.socket";  
char *server_path = "cpfuzz.socket";  
double result = DBL_MAX;

int main(int argc, char** argv) {
  int sim_time;
  size_t input_arr_num, int_state_arr_num, float_state_arr_num, x_arr_num, output_arr_num;
  INPUT_VAL iv;
  RETURN_VAL rv;
  struct  sockaddr_un cliun, serun;  
  int len;  
  double buf[100];  
  double *input_buf;
  double rob;
  int sockfd, n, i;  

  if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){  
      perror("client socket error");  
      exit(1);  
  }  
      
  memset(&cliun, 0, sizeof(cliun));  
  cliun.sun_family = AF_UNIX;  
  strcpy(cliun.sun_path, client_path);  
  len = offsetof(struct sockaddr_un, sun_path) + strlen(cliun.sun_path);  
  unlink(cliun.sun_path);  
  if (bind(sockfd, (struct sockaddr *)&cliun, len) < 0) {  
      perror("bind error");  
      exit(1);  
  }  

  memset(&serun, 0, sizeof(serun));  
  serun.sun_family = AF_UNIX;  
  strcpy(serun.sun_path, server_path);  
  len = offsetof(struct sockaddr_un, sun_path) + strlen(serun.sun_path);  
  if (connect(sockfd, (struct sockaddr *)&serun, len) < 0){  
      perror("connect error");  
      exit(1);  
  }  

  if(argc == 7) {
		sim_time = atoi(argv[1]);
        input_arr_num = atoi(argv[2])*sizeof(double);
        input_buf = (double*)malloc(input_arr_num * sim_time);

        int_state_arr_num = atoi(argv[3])*sizeof(int);
        iv.int_state_arr = (int*)malloc(int_state_arr_num);
        rv.int_state_arr = iv.int_state_arr;

        float_state_arr_num = atoi(argv[4])*sizeof(double);
        iv.float_state_arr = (double*)malloc(float_state_arr_num);
        rv.float_state_arr = iv.float_state_arr;

        x_arr_num = atoi(argv[5])*sizeof(double);
        iv.x_arr = (double*)malloc(x_arr_num);

        output_arr_num = atoi(argv[6]) *sizeof(double);
        rv.output_arr = (double*)malloc(output_arr_num);

	} else {
		printf("Usage: %s sim_time input_arr_num int_state_arr_num float_state_arr_num x_arr_num output_arr_num\n", argv[0]);
        exit(0);
	}

    memset(input_buf, 0, input_arr_num * sim_time);
    memset(iv.int_state_arr, 0, int_state_arr_num);
    memset(iv.float_state_arr, 0, float_state_arr_num);
    memset(iv.x_arr, 0, x_arr_num);
    memset(rv.output_arr, 0, output_arr_num);

    n = read(STDIN_FILENO, input_buf, input_arr_num * sim_time);
    iv.input_arr = input_buf;
    
    n = read(STDIN_FILENO, iv.x_arr, x_arr_num);
 
    // return init state
    n = write(sockfd, iv.x_arr, x_arr_num);   

    
    
    controller_init();
    while (sim_time--) {

        controller(&iv, &rv);
        iv.input_arr += (input_arr_num >> 3);
        // return control output
        n = write(sockfd, rv.output_arr, output_arr_num);    

        // update state
        n = read(sockfd, iv.x_arr, x_arr_num);    
        if ( n < 0 ) {    
          printf("the other side has been closed.%d\n",n);    
        }
        //  calc robustness of new state

        rob = -DBL_MAX;
        rob = MAX(rob, MAX(iv.x_arr[0] -  DBL_MAX, (-DBL_MAX) - iv.x_arr[0] ));
rob = MAX(rob, MAX(iv.x_arr[1] -  DBL_MAX, (-DBL_MAX) - iv.x_arr[1] ));
rob = MAX(rob, MAX(iv.x_arr[2] -  DBL_MAX, (-DBL_MAX) - iv.x_arr[2] ));
rob = MAX(rob, MAX(iv.x_arr[3] -  -8.0, (-DBL_MAX) - iv.x_arr[3] ));
        result = MIN(result, rob);

        if (result < 0){
            // after find a violation, test the violating state
            for(i=0; i < x_arr_num/sizeof(double);i++){
                printf("x[%d] = %lf\n",i, iv.x_arr[i]);
            }
            printf("simtime = %d, robust = %lf\n", sim_time, result);

            break;
        } 
    
    }
    close(sockfd); 
    free(input_buf);
    free(iv.int_state_arr);
    free(iv.float_state_arr);
    free(iv.x_arr);
    free(rv.output_arr);
    asm  (  "movq result, %rax;\
            leaq __afl_global_area_ptr, %rdx; \
            movq  0(%rdx), %rbx; \
            movq  %rax, 8192(%rbx)");
    return 0;
}