
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

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

int main(void)
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

    while(i < 21)
    {
        err = pthread_create(&(tid[i]), NULL, &doSomeThing, NULL);
        if (err != 0)
            printf("can't create thread :[%s]\n", strerror(err));
        else
            printf("Thread %d created successfully\n", i);

        i++;
    }

    printf("%s is now done.... waiting for workers\n", thread_name);

    for ( k = 1 ; k < 21; k++){
       pthread_join(tid[k], NULL);
    }

    return 0;
}