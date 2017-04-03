
#include<pthread.h>
#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

pthread_t tid[20];

void* doSomeThing(void *arg)
{
    int i = 0;
    int j = 1;
    pthread_t id = pthread_self();

    while(!pthread_equal(id, tid[j]))
    {
        j++;
    }

    printf("Thread Number %d processing\n", j);

    // if(pthread_equal(id,tid[0]))
    // {
    //     printf("\n First thread processing\n");
    // }
    // else
    // {
    //     printf("\n Second thread processing\n");
    // }

    //for(i=0; i<(0xFFFFFFFF);i++);

    for(i=0; i< 10;i++){sleep(1);};

    printf("Thread Number %d finished\n", j);

    return NULL;
}

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

   int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, status = 1, flag = 1;
     socklen_t clilen;
     char buffer[256];
     int clients[] = {0,0,0,0};
     struct sockaddr_in serv_addr, cli_addr;
     int n;

     int i = 1, k = 1;
     int err;
     pthread_t thread;
     char thread_name[16];


///////////////////////////////////////////////////////////////////// Rename master thread
    thread = pthread_self();
    err = pthread_setname_np(thread, "Master");
    if (err != 0)
            printf("can't rename thread :[%s]\n", strerror(err));
    else
            printf("Thread renamed successfully\n");

    err = pthread_getname_np(thread, thread_name, 16);
    if (err != 0)
            printf("can't get thread name :[%s]\n", strerror(err));

    printf("\n");

////////////////////////////////////////////////////////////////////// Create worker threads


    while(i < 21)
    {
        err = pthread_create(&(tid[i]), NULL, &doSomeThing, NULL);
        if (err != 0)
            printf("can't create thread :[%s]\n", strerror(err));
        else
            printf("Thread %d created successfully\n", i);

        i++;
    }

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
     




/////////////////////////////////////////////////////// Main thread accepting and storing client fds     
     flag = 1;
     int queue = 1;
     while(status)
     {
        clilen = sizeof(cli_addr);
        

        for(queue = 0; flag == 1; queue++){
    
            if(queue== 4){printf("Queue Full\n" );flag =0;}
            else if(clients[queue] == 0){     
            clients[queue] = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, &clilen);

            printf("\rNumber of connections: %d",queue);
            if(clients[queue] < 0) 
                error("ERROR on accept");
            } 
        }
    }

     // bzero(buffer,256);
     // n = read(newsockfd,buffer,255);
     // if (n < 0) error("ERROR reading from socket");
     // printf("Here is the message: %s\n",buffer);
     // n = write(newsockfd,"I got your message",18);
     // if (n < 0) error("ERROR writing to socket");
     // close(newsockfd);
     // close(sockfd);

     // return 0;


    printf("%s is now done.... waiting for workers\n", thread_name);

    for ( k = 1 ; k < 21; k++){
       pthread_join(tid[k], NULL);
    }

    return 0;


}