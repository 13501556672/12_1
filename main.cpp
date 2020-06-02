#include <pthread.h>
#include <sys/types.h>       /* some systems still require this */
#include <stdio.h>     /* for convenience */
#include <stdlib.h>    /* for convenience */
#include <stddef.h>    /* for offsetof */
#include <string.h>    /* for convenience */
#include <unistd.h>    /* for convenience */
#include <signal.h>    /* for SIG_ERR */
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

void prepare(void)
{
	int err;

	printf("preparing locking ...\n");
	if ((err = pthread_mutex_lock(&lock1)) != 0)
		printf("can't lock lock1 in prepare handler");
	if ((err = pthread_mutex_lock(&lock2)) != 0)
		printf("can't lock lock1 in prepare handler");
}

void parent(void)
{
	int err;
	printf("parent is unlocking ...\n");
	if((err = pthread_mutex_unlock(&lock1)) != 0)
		printf("can't unlock lock1 in parent handler");
	if ((err = pthread_mutex_unlock(&lock2)) != 0)
		printf("can't unlock lock2 in prepare handler");
}

void child(void)
{
	int err;
	printf("child is unlocking ...\n");
	if ((err = pthread_mutex_unlock(&lock1)) != 0)
		printf("can't unlock lock1 in child handler");
	if ((err = pthread_mutex_unlock(&lock2)) != 0)
		printf("can't unlock lock2 in dhild handler");
}

void* thr_fn(void* arg)
{
	printf("thr_fn is coming\n");
	pause();
	return (void*)0;
}
int main()
{
	int err;
	pid_t pid;
	pthread_t tid;

	if ((err = pthread_atfork(prepare, parent, child) != 0))
	{
		printf("can't install fork handles\n");
		exit(1);//程序异常退出
	}
	if ((err = pthread_create(&tid, NULL, thr_fn, 0)) != 0)
	{
		printf("can't create thread\n");
		exit(1);
	}
	sleep(2);  //进程先执行
	printf("parent is about to fork ...\n");

	if ((pid = fork()) < 0)
		exit(1);
	else if (pid == 0)
		printf("child returned from fork\n");
	else
	{
		printf("parent returned from fork\n");
	}
	return 0;

}