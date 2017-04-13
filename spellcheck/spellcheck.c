
#include<pthread.h>
#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <semaphore.h>

pthread_t tid[20];
int client_count = 0, in = 0, out = 0, full = 0;
int clients[49];
FILE *fp;
pthread_mutex_t client_mutex, service_mutex, count_mutex;
typedef sem_t Semaphore;

Semaphore *make_sem(int value)
{
    Semaphore *sem = malloc(sizeof(Semaphore));
    int n = sem_init(sem, 0, value);
    if(n != 0){perror("sem failed to initialize");}
    return sem;
}

void semaphore_wait(Semaphore *sem)
{
    int n = sem_wait(sem);
    if(n != 0){perror("sem_wait failed");}
}

void semaphore_signal(Semaphore *sem)
{
    int n = sem_post(sem);
    if(n != 0){perror("sem_post failed");}

}
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void add_client(int client_fd)
{
    pthread_mutex_lock(&client_mutex);
    clients[in%50] = client_fd;
    pthread_mutex_lock(&count_mutex);
    client_count = client_count + 1;
    if(client_count >49)
        full = 1;
    pthread_mutex_unlock(&count_mutex);
    in = in + 1;
    //printf("client added!\n");
    pthread_mutex_unlock(&client_mutex);
}

int service_client()
{
     int new_out;
     
     if(client_count > 0)
     {
     pthread_mutex_lock(&service_mutex);
     pthread_mutex_lock(&count_mutex);
     client_count = client_count - 1;
     full = 0;
     pthread_mutex_unlock(&count_mutex);
     new_out = clients[out%50];
     clients[out%50] = 0;
     out = out+1;
     //printf("Out position: %d\n", out%50);
     //printf("client_count: %d\n", client_count);
     pthread_mutex_unlock(&service_mutex);
     return new_out;
     }
     return 0;
}

////////////////////////////////////////////////////////// Perform spellcheck
int check(int filep)
{
 
 char buffer[255];
 char file_buff[255];
 int n =0, k = 0;

    fp = fopen("dictionary.txt", "r" );
    //printf("fd: %d\n", fd );
    bzero(buffer,255);
    bzero(file_buff,255);
    n = read(filep,buffer,255);
    if (n < 0) error("ERROR reading from socket");
    while(fgets(file_buff, 255, fp))
    {  
        
        if(!strcmp(buffer, file_buff))
        {
            n = write(filep,"Word spelled correctly",36);
            fclose(fp);
            return 1;
            k++;
        }             
    }
    n = write(filep,"Word Not found \n",18);
    return 1;
    

}

//////////////////////////////////////////////////////////////// Thread Function
void* doSomeThing(void *arg)
{
    int fd =0, j = 0, n = 0;
    
    pthread_t id = pthread_self();
    while(!pthread_equal(id, tid[j]))
    {j++;}

    while(1)
    {
        if(client_count > 0)
        {
            fd = service_client();
            if(fd != 0)
            {
                check(fd);                                  ///////Call check function

            if (n < 0){ error("ERROR writing to socket");}
                close(fd);
            }
                    // int g = 0;
                    // for(g = 0; g < 49; g++){printf("%d ", clients[g]);}
                    //     printf("\n");
        }

    }

     return NULL;
}

//////////////////////////////////////////////////////////////////// Start Main Function 
int main(int argc, char *argv[])
{
     int sockfd, portno;
     socklen_t clilen;
     
     
     struct sockaddr_in serv_addr, cli_addr;

     int k = 1, y = 0;;
     int err;
     pthread_t thread;
     char thread_name[16];
     in = 0;
     out = 0;

///////////////////////////////////////////////////////////////////// Rename master thread
    
    thread = pthread_self();
    err = pthread_setname_np(thread, "Master");
    if (err != 0)
            printf("can't rename thread :[%s]\n", strerror(err));
    // else
    //         printf("Thread renamed successfully\n");

    err = pthread_getname_np(thread, thread_name, 16);
    if (err != 0)
            printf("can't get thread name :[%s]\n", strerror(err));

//////////////////////////////////////////////////////Start Server... Create Socket and bind
     
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     //**Create socket**
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");

     bzero((char *) &serv_addr, sizeof(serv_addr));     // Initialize server address to 0     
     portno = atoi(argv[1]);                            //Set port to arg 1
     
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);                                   //Set port Listener
     //printf("binding complete\n");     
////////////////////////////////////////////////////////////////////// Create worker threads
    while(y < 5)
    {
        err = pthread_create(&(tid[y]), NULL, &doSomeThing, NULL);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        else
            printf("\rThread created successfully");
        y++;
    }

/////////////////////////////////////////////////////// Main thread accepting and storing client fds
     //int flag = 1;
     int client_fd;
     clilen = sizeof(cli_addr);

     while(1)
     {
    
        if(full)
        {
            //stop producing if full   
        }               
        else if(clients[in%50] == 0)
        {     
        client_fd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if(clients[in%50] < 0) 
            error("ERROR on accept");
             
        add_client(client_fd);

        }
    }

    // printf("\r%s is now done.... waiting for workers", thread_name);


    for ( k = 1 ; k < 5; k++){
       pthread_join(tid[k], NULL);
    }

    fclose(fp);
    return 0;


}