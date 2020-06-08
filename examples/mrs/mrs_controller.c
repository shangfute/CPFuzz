
#include "controller.h"

#define H (90)
#define L (10)

#define DEBUG
void controller_init(){
}

void* controller(INPUT_VAL* input, RETURN_VAL* ret_val)
{
  double xB = input->x_arr[2];
  double xA = input->x_arr[1];

  double w1 = input->input_arr[0];
  double w2 = input->input_arr[1];
  double w3 = input->input_arr[2];
  double w4 = input->input_arr[3];
  double w5 = input->input_arr[4];
  double w6 = input->input_arr[5];
  double w7 = input->input_arr[6];
  double w8 = input->input_arr[7];

  double Y;
  int i;

#ifdef DEBUG
  for(i=0; i<8;i++)
      printf("%lf ", input->input_arr[i]);
  printf("\n");
  printf("%lf %lf\n", xA, xB);
#endif
  Y = xA;
  if (w1 < H){
    Y = xB;
    asm ( 
        "leaq -(128+24)(%rsp), %rsp; \
        movq %rdx,  0(%rsp); \
        movq %rcx,  8(%rsp); \
        movq %rax, 16(%rsp); \
        movq $0x08, %rcx; \
        call __afl_maybe_log; \
        movq 16(%rsp), %rax; \
        movq  8(%rsp), %rcx; \
        movq  0(%rsp), %rdx; \
        leaq (128+24)(%rsp), %rsp"
      );
    }
  else{
    asm ( 
        "leaq -(128+24)(%rsp), %rsp; \
        movq %rdx,  0(%rsp); \
        movq %rcx,  8(%rsp); \
        movq %rax, 16(%rsp); \
        movq $0x08, %rcx; \
        call __afl_maybe_log; \
        movq 16(%rsp), %rax; \
        movq  8(%rsp), %rcx; \
        movq  0(%rsp), %rdx; \
        leaq (128+24)(%rsp), %rsp"
      );
  }

  if (w2 > L) {
    Y = xB;
    asm ( 
        "leaq -(128+24)(%rsp), %rsp; \
        movq %rdx,  0(%rsp); \
        movq %rcx,  8(%rsp); \
        movq %rax, 16(%rsp); \
        movq $0x08, %rcx; \
        call __afl_maybe_log; \
        movq 16(%rsp), %rax; \
        movq  8(%rsp), %rcx; \
        movq  0(%rsp), %rdx; \
        leaq (128+24)(%rsp), %rsp"
      );
    }
  else{
    asm ( 
        "leaq -(128+24)(%rsp), %rsp; \
        movq %rdx,  0(%rsp); \
        movq %rcx,  8(%rsp); \
        movq %rax, 16(%rsp); \
        movq $0x08, %rcx; \
        call __afl_maybe_log; \
        movq 16(%rsp), %rax; \
        movq  8(%rsp), %rcx; \
        movq  0(%rsp), %rdx; \
        leaq (128+24)(%rsp), %rsp"
      );
  }

  if (w3 < H){
    Y = xB;
    asm ( 
        "leaq -(128+24)(%rsp), %rsp; \
        movq %rdx,  0(%rsp); \
        movq %rcx,  8(%rsp); \
        movq %rax, 16(%rsp); \
        movq $0x08, %rcx; \
        call __afl_maybe_log; \
        movq 16(%rsp), %rax; \
        movq  8(%rsp), %rcx; \
        movq  0(%rsp), %rdx; \
        leaq (128+24)(%rsp), %rsp"
      );
    }
  else{
    asm ( 
        "leaq -(128+24)(%rsp), %rsp; \
        movq %rdx,  0(%rsp); \
        movq %rcx,  8(%rsp); \
        movq %rax, 16(%rsp); \
        movq $0x08, %rcx; \
        call __afl_maybe_log; \
        movq 16(%rsp), %rax; \
        movq  8(%rsp), %rcx; \
        movq  0(%rsp), %rdx; \
        leaq (128+24)(%rsp), %rsp"
      );
  }
  if (w4 > L){
    Y = xB;
    asm ( 
        "leaq -(128+24)(%rsp), %rsp; \
        movq %rdx,  0(%rsp); \
        movq %rcx,  8(%rsp); \
        movq %rax, 16(%rsp); \
        movq $0x08, %rcx; \
        call __afl_maybe_log; \
        movq 16(%rsp), %rax; \
        movq  8(%rsp), %rcx; \
        movq  0(%rsp), %rdx; \
        leaq (128+24)(%rsp), %rsp"
      );
    }
  else{
    asm ( 
        "leaq -(128+24)(%rsp), %rsp; \
        movq %rdx,  0(%rsp); \
        movq %rcx,  8(%rsp); \
        movq %rax, 16(%rsp); \
        movq $0x08, %rcx; \
        call __afl_maybe_log; \
        movq 16(%rsp), %rax; \
        movq  8(%rsp), %rcx; \
        movq  0(%rsp), %rdx; \
        leaq (128+24)(%rsp), %rsp"
      );
  }
  if (w5 < H){
    Y = xB;
    asm ( 
        "leaq -(128+24)(%rsp), %rsp; \
        movq %rdx,  0(%rsp); \
        movq %rcx,  8(%rsp); \
        movq %rax, 16(%rsp); \
        movq $0x08, %rcx; \
        call __afl_maybe_log; \
        movq 16(%rsp), %rax; \
        movq  8(%rsp), %rcx; \
        movq  0(%rsp), %rdx; \
        leaq (128+24)(%rsp), %rsp"
      );
    }
  else{
    asm ( 
        "leaq -(128+24)(%rsp), %rsp; \
        movq %rdx,  0(%rsp); \
        movq %rcx,  8(%rsp); \
        movq %rax, 16(%rsp); \
        movq $0x08, %rcx; \
        call __afl_maybe_log; \
        movq 16(%rsp), %rax; \
        movq  8(%rsp), %rcx; \
        movq  0(%rsp), %rdx; \
        leaq (128+24)(%rsp), %rsp"
      );
  }
  if (w6 > L){
    Y = xB;
    asm ( 
        "leaq -(128+24)(%rsp), %rsp; \
        movq %rdx,  0(%rsp); \
        movq %rcx,  8(%rsp); \
        movq %rax, 16(%rsp); \
        movq $0x08, %rcx; \
        call __afl_maybe_log; \
        movq 16(%rsp), %rax; \
        movq  8(%rsp), %rcx; \
        movq  0(%rsp), %rdx; \
        leaq (128+24)(%rsp), %rsp"
      );
    }
  else{
    asm ( 
        "leaq -(128+24)(%rsp), %rsp; \
        movq %rdx,  0(%rsp); \
        movq %rcx,  8(%rsp); \
        movq %rax, 16(%rsp); \
        movq $0x08, %rcx; \
        call __afl_maybe_log; \
        movq 16(%rsp), %rax; \
        movq  8(%rsp), %rcx; \
        movq  0(%rsp), %rdx; \
        leaq (128+24)(%rsp), %rsp"
      );
  }

  if (w7 < H){
    Y = xB;
    asm ( 
        "leaq -(128+24)(%rsp), %rsp; \
        movq %rdx,  0(%rsp); \
        movq %rcx,  8(%rsp); \
        movq %rax, 16(%rsp); \
        movq $0x08, %rcx; \
        call __afl_maybe_log; \
        movq 16(%rsp), %rax; \
        movq  8(%rsp), %rcx; \
        movq  0(%rsp), %rdx; \
        leaq (128+24)(%rsp), %rsp"
      );
    }
  else{
    asm ( 
        "leaq -(128+24)(%rsp), %rsp; \
        movq %rdx,  0(%rsp); \
        movq %rcx,  8(%rsp); \
        movq %rax, 16(%rsp); \
        movq $0x08, %rcx; \
        call __afl_maybe_log; \
        movq 16(%rsp), %rax; \
        movq  8(%rsp), %rcx; \
        movq  0(%rsp), %rdx; \
        leaq (128+24)(%rsp), %rsp"
      );
  }
  if (w8 > L){
    Y = xB;
    asm ( 
        "leaq -(128+24)(%rsp), %rsp; \
        movq %rdx,  0(%rsp); \
        movq %rcx,  8(%rsp); \
        movq %rax, 16(%rsp); \
        movq $0x08, %rcx; \
        call __afl_maybe_log; \
        movq 16(%rsp), %rax; \
        movq  8(%rsp), %rcx; \
        movq  0(%rsp), %rdx; \
        leaq (128+24)(%rsp), %rsp"
      );
    }
  else{
    asm ( 
        "leaq -(128+24)(%rsp), %rsp; \
        movq %rdx,  0(%rsp); \
        movq %rcx,  8(%rsp); \
        movq %rax, 16(%rsp); \
        movq $0x08, %rcx; \
        call __afl_maybe_log; \
        movq 16(%rsp), %rax; \
        movq  8(%rsp), %rcx; \
        movq  0(%rsp), %rdx; \
        leaq (128+24)(%rsp), %rsp"
      );
  }
//  if ((w1 < H) || (w2 > L) 
//          || ((w3 < H) || (w4 > L)) 
//          || ((w5 < H) || (w6 > L)) 
//          || ((w7 < H) || (w8 > L))) {

//           Y = xB;
//         } else {
//           Y = xA;
//         }


  ret_val->output_arr[0] = Y;

  return (void*)0;
}
