#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(){
pid_t pid;
pid = fork();
if (pid > 0){
fork();
while(1){
printf ("%d I am the parent of pid=%d!\n",getpid(),getppid());
sleep(10);
}
}
else if (!pid){
pid = fork();
if(pid > 0)
fork();
while(1){
printf ("%d I am the parent of pid=%d!\n",getpid(),getppid());
sleep(10);
}
}
else if (pid == -1)
perror (��fork��);
return 0;
}

