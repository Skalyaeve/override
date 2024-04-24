#include <sys/ptrace.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/prctl.h>

#define CHILD 0x0
//                            STACK GROW (0xffffffff => 0x00000000)     /\  ARG 1 = ... to 0x08(%ebp)
int main(void) // ARGS        || 0x04(%ebp) ==> 0x08(%ebp)             /  \ ARG 0 = 0x08(%ebp) to 0x04(%ebp)
{              // SAVED EIP   || 0x00(%ebp) ==> 0x04(%ebp)              ||  return address
        //------- EBP (GAP)   || 0xb0(%esp) ==> 0x00(%ebp) = ? bytes    ||  saved registers + gap
        __pid_t pid;    //--- || 0xac(%esp) ==> 0xb0(%esp) = 4 bytes    ||  local variables
        int ptrace_ret; //--- || 0xa8(%esp) ==> 0xac(%esp) = 4 bytes    ||  ...
        int temp_int1;  //--- || 0xa4(%esp) ==> 0xa8(%esp) = 4 bytes    ||  F ...
        int temp_int2;  //--- || 0xa0(%esp) ==> 0xa4(%esp) = 4 bytes    ||  E 0X42
        //------------ GAP ?: || 0x40(%esp) ==> 0xa0(%esp) = 96 bytes   ||  D 0X2a
        char buffer[0x20]; // || 0x20(%esp) ==> 0x40(%esp) = 32 bytes ? ||  C 0Xff
        int wait_status;   // || 0x1c(%esp) ==> 0x20(%esp) = 4 bytes    ||  B 0Xbf ==> 0xbfff2a42...
        //------- ESP (GAP)   || 0x00(%esp) ==> 0x1c(%esp) = 28 bytes   ||  A ==> ABCDEF...
        // sub 0xb0, esp     \  /                                       ||
        //                    \/                                READ/WRITE
        pid = fork();
        for (int i = 0x0; i < 0x20; i++)
                buffer[i] = 0x0;
        ptrace_ret = 0x0;
        wait_status = 0x0;
        if (pid == CHILD)
        {
                prctl(PR_SET_PDEATHSIG, 0x1);
                ptrace(PTRACE_TRACEME, 0x0, 0x0, 0x0);
                puts("Give me some shellcode, k");
                gets(buffer);
        }
        else
        {
                do
                {
                        wait(&wait_status);
                        if (((temp_int2 = wait_status, temp_int2 & 0x7F) == 0x0) ||
                            ((temp_int1 = wait_status, (temp_int1 & 0x7F) + 0x1) >> 0x1) > 0x0)
                        {
                                puts("child is exiting...");
                                return 0x0;
                        }
                        ptrace_ret = ptrace(PTRACE_PEEKUSER, pid, 0x2c, 0x0);
                } while (ptrace_ret != 0xb);

                puts("no exec() for you");
                kill(pid, 0x9);
        }
        return 0x0;
}
