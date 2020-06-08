#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>  
#include <stddef.h>  
#include <sys/socket.h>  
#include <sys/un.h>  

#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/file.h>
#include <errno.h>  
#include <sys/types.h>
#include <dirent.h>

  
int main(int argc, char** argv) {
  
  int len, fd, i, j;
  uint8_t  *in_buf;
  double data;
  DIR* sd;
  struct dirent* sd_ent;
  char path[256];
  char cmd[512];

  sd = opendir(argv[1]);
  if (!sd) printf("Unable to open '%s'", argv[1]);

  while ((sd_ent = readdir(sd))) {
      // if (sd_ent->d_name[0]=='.' || !strstr(sd_ent->d_name, "+ce")) continue;
      if (sd_ent->d_name[0]=='.' ) continue;

      sprintf(path,"%s/%s", argv[1], sd_ent->d_name);

      /* Allow this to fail in case the other fuzzer is resuming or so... */

      fd = open(path, O_RDONLY);
      printf("%s\nInputs:\n",path);


    if (fd < 0) printf("Unable to open '%s'", argv[1]);


    len = read(fd,&data,8);
    while(len==8){
      printf("%lf ",data);
      len = read(fd,&data,8);
    } 

    close(fd);
    printf("\n");
    // strcpy(cmd,       "/home/shang/CPFuzz/examples/spi/fuzz-target 50 1 0 0 1 1 < ");
    // strcpy(cmd,       "/home/shang/CPFuzz/examples/spi/fuzz-target 200 1 0 0 1 1 < ");
    // strcpy(cmd,       "/home/shang/CPFuzz/examples/spi/fuzz-target 500 1 0 0 1 1 < ");
    // strcpy(cmd,  "/home/shang/CPFuzz/examples/dc_motor/fuzz-target 50 1 0 1 2 1 < ");
    // strcpy(cmd,"/home/shang/CPFuzz/examples/fuzzy_invp/fuzz-target 10 1 0 0 3 1 < ");
    // strcpy(cmd,      "/home/shang/CPFuzz/examples/heat/fuzz-target 20 0 3 0 3 3 < ");
    // strcpy(cmd,    "/home/shang/CPFuzz/examples/heater/fuzz-target 50 0 4 0 1 1 < ");
    // strcpy(cmd,    " /home/shang/CPFuzz/examples/CSTR/fuzz-target 9 1 1 0 2 1 < ");
    strcpy(cmd,    " /home/shang/CPFuzz/examples/cstr2/fuzz-target 9 1 1 0 2 1 < ");
    // strcpy(cmd,          "/home/shang/CPFuzz/examples/mrs/fuzz-target 2  8 0 0 4 1 < ");
    
    strcat(cmd,path);
      printf("Output:\n");
    system( cmd);
  }
  
  printf("\n");
}