#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void* doSomeThing(void *arg){

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);
    close(sockfd);
    return NULL;
 }

int main(int argc, char *argv[])
{

    int i = 1, k = 1;
    int err;
    pthread_t thread;
    char thread_name[16];

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

    while(i < 5)
    {
        err = pthread_create(&(tid[i]), NULL, &doSomeThing, NULL);
        if (err != 0)
            printf("can't create thread :[%s]\n", strerror(err));
        else
            printf("Thread %d created successfully\n", i);

        i++;
    }

    printf("%s is now done.... waiting for workers\n", thread_name);

    for ( k = 1 ; k < 5; k++){
       pthread_join(tid[k], NULL);
    }

    return 0;
}