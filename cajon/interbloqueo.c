#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

pthread_mutex_t lockA = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lockB = PTHREAD_MUTEX_INITIALIZER;

void *thread_funcA(void *arg)
{
    int ret;

    printf("Hilo A intentando lockA...\n");
    ret = pthread_mutex_lock(&lockA);
    printf("Hilo A lockA ret=%d\n", ret);

    sleep(1);

    printf("Hilo A intentando lockB...\n");
    ret = pthread_mutex_lock(&lockB);
    if (ret != 0)
        printf("Hilo A lockB error=%d (errno=%d)\n", ret, errno);

    // Nunca se llega aquí si entra en deadlock
    pthread_mutex_unlock(&lockB);
    pthread_mutex_unlock(&lockA);
    return NULL;
}

void *thread_funcB(void *arg)
{
    int ret;

    printf("Hilo B intentando lockB...\n");
    ret = pthread_mutex_lock(&lockB);
    printf("Hilo B lockB ret=%d\n", ret);

    sleep(1);

    printf("Hilo B intentando lockA...\n");
    ret = pthread_mutex_lock(&lockA);
    if (ret != 0)
        printf("Hilo B lockA error=%d (errno=%d)\n", ret, errno);

    pthread_mutex_unlock(&lockA);
    pthread_mutex_unlock(&lockB);
    return NULL;
}

int main(void)
{
    pthread_t tA, tB;

    pthread_create(&tA, NULL, thread_funcA, NULL);
    pthread_create(&tB, NULL, thread_funcB, NULL);

    pthread_join(tA, NULL);
    pthread_join(tB, NULL);

    return 0;
}
