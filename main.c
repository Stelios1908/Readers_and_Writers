#include <stdio.h>       //print
#include <stdlib.h>        //exit(),maloc,free
#include <sys/types.h>   //key_t,sem_t,pid_t
#include <sys/shm.h>         //shmat(),smget()
#include <errno.h>       //
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>       //c;ock()


int main()
{
typedef sem_t semaphore;
semaphore *mutex,*data;
mutex =sem_open("sem1",O_CREAT|O_EXCL,0644,1);
data  =sem_open("sem2",O_CREAT|O_EXCL,0644,1);

key_t key1,key2,key3;
key1=ftok("/dev,null",7);   
key2=ftok("/dev,/null",7);
key3= ftok("/dev/null",7);

int smid1=shmget(12345673,sizeof(int),IPC_CREAT|0644); //  write count
int smid2=shmget(83736252,sizeof(int),IPC_CREAT|0644); //  read count
int smid3=shmget(35352636,sizeof(int),IPC_CREAT|0644); // priority

int *wc,*rc,*pr;
wc=(int *) shmat(smid1,NULL,0);    //pointer   
rc=(int *) shmat(smid2,NULL,0);
pr=(int *) shmat(smid3,NULL,0);
*wc=0;                            //  
*rc=0;
*pr=0;
int status;

int i;
int n;
printf("\ngive number process child you wont : ");
scanf("%d",&n);
pid_t pid[n];

for(i=0;i<n;i++)
  {
       
    pid[i]=fork();    // 
    
    //       readers
      if(pid[i]==0 && i%2==1)   
         { printf("\ndiergasia me id : %d\n",getpid());
             
           while(1)
           { 
             sem_wait(mutex);        //up
             *rc=*rc+1;
             sem_post(mutex); 
             sem_wait(data);
             
             if(*pr==0)
               {
                   printf("proc with %d kai grafo \n",i);
                   sem_wait(mutex);        //up
                   *rc=*rc-1;
                   if(*wc!=0)
                   {
                   *pr=1;
                   }
                   sem_post(mutex);   //down
                   sem_post(data);
                   exit(0);
               }
               else
               {
                   sem_wait(mutex);
                   if(*wc==0)
                   {
                    *pr=0;
                   }
                    *rc=*rc-1;
                  sem_post(mutex);
               }
               sem_post(data);
            }
         }
         //writers
        if(pid[i]==0 && i%2==0)   
         { printf("\nproc with id : %d\n",getpid());
             
           while(1)
           { 
             sem_wait(mutex);        //up
             *wc=*wc+1;
             sem_post(mutex); 
             sem_wait(data);
             
             if(*pr==0)
               {
                   printf("eimai  diaergasia %d kai diabazo \n",i);
                   sem_wait(mutex);        //up
                   *wc=*wc-1;
                   if(*rc!=0)
                   {
                   *pr=1;
                   }
                   sem_post(mutex);   //down
                   sem_post(data);
                   exit(0);
               }
               else
               {
                   sem_wait(mutex);
                   if(*rc==0)
                   {
                    *pr=0;
                   }
                    *wc=*wc-1;
                  sem_post(mutex);
               }
               sem_post(data);
            }
        }
    }
      
      system("ps -l");
      int whodie[n];
         for(i=0;i<n;i++)
           {
           
            whodie[i]=waitpid(pid[i],&status,0);
            printf("the process with id %d exited  ",whodie[i]);
               if (!WIFEXITED(status)) 
                {
                printf ("abnormally!\n");
                }
               else 
                {
               printf ("with status %d.\n", WEXITSTATUS(status));
                } 
           
            }
       sem_unlink("sem1");
       sem_close(mutex);
       sem_unlink("sem2");
       sem_close(data);
      	return (0);
}