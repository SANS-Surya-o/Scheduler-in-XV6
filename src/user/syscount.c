#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/syscall.h"

#define false 0
#define true 1

// An array of strings mapping syscall numbers to names
char *syscall_names[NUM_SYSCALLS] = {
  [SYS_fork]    = "fork",
  [SYS_exit]    = "exit",
  [SYS_wait]    = "wait",
  [SYS_pipe]    = "pipe",
  [SYS_read]    = "read",
  [SYS_kill]    = "kill",
  [SYS_exec]    = "exec",
  [SYS_fstat]   = "fstat",
  [SYS_chdir]   = "chdir",
  [SYS_dup]     = "dup",
  [SYS_getpid]  = "getpid",
  [SYS_sbrk]    = "sbrk",
  [SYS_sleep]   = "sleep",
  [SYS_uptime]  = "uptime",
  [SYS_open]    = "open",
  [SYS_write]   = "write",
  [SYS_mknod]   = "mknod",
  [SYS_unlink]  = "unlink",
  [SYS_link]    = "link",
  [SYS_mkdir]   = "mkdir",
  [SYS_close]   = "close",
  [SYS_waitx]   = "waitx",
  [SYS_getSysCount] = "getSysCount",
  [SYS_sigalarm] = "sigalarm",
  [SYS_sigreturn] = "sigreturn",
  [SYS_settickets] = "settickets",
  
};


int isnum(char *s)
{
    for (int i = 0; s[i] != '\0'; i++)
    {
        if (s[i] < '0' || s[i] > '9')
        {
            return false;
        }
    }
    return true;
}

int main(int argc , char* argv[])
{
    if (argc < 3) {
        fprintf(2, "Usage: syscount <mask> command [args]\n");
        exit(0);
    }

    if (!isnum(argv[1])) {
        fprintf(2, "syscount: invalid mask\n");
        exit(1);
    }
    int mask = atoi(argv[1]);
    int num = -1;
    for (int i=1;i<=31;i++)
    {
        if (mask == (1<<i))
        {
            num = i;
            break;
        }
    }
    if (num==-1)
    {
        printf("Invalid mask\n");
        exit(1);
    }
    
    int pid = fork();
     if (pid < 0) {
        fprintf(2,"Fork failed!\n");
        exit(1);
    } else if (pid == 0) {
        // Child process: execute the command
        exec(argv[2], &argv[2]);
        fprintf(2, "Exec failed!: %s\n",argv[2]);
        exit(1);
    } else {
        // Parent process: wait for child to complete and get syscall count
        int status;
        wait(&status);
        int count = getSysCount(num);
        if (num == 1)
        {
            // A fork was called by the syscount process itself
            count--;
        }

        if (count < 0) {
            fprintf(2, "Invalid system call mask!\n");
        } else {
            printf("Process %d called %s %d times\n",pid,syscall_names[num],count);
        }
    }

    exit(1);
}



