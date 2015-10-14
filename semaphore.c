/*
 name        : demo_fork.c
 Author      : panpeng
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

const char *ftpath	="/home/deepin/桌面/he.c";
#define		PROJ_ID		1			// ftok的项目数

union semun
{
	int					val;				//for SETVAL
	struct semid_ds		*buf;				//for IPC_STAT and IPC_SET
	unsigned short		*array;				//for GETALL and SETALL
	};

int main(void)
{
	pid_t			pid;
	key_t			ipc_key;
	int				semid;										// semaphore set identifier
	union semun		arg;										// parameter for initialize semaphore vaule
	struct sembuf	child_sops, parent_sops;


	int 			value;

	if(-1 == (ipc_key =ftok(ftpath, PROJ_ID)) )					// creat an ipc key
	{
		fprintf(stderr,"Ftok Error:%s\a\n",strerror(errno));
		exit(1);
	}
	if(-1 == (semid =semget(ipc_key, 1, IPC_CREAT)))			// creat a  semaphore set with 1 number
	{
		fprintf(stderr,"semget Error:%s\a\n",strerror(errno));
		exit(1);
	}
	//int semctl(int semid, int semnum, int cmd, ...);
	arg.val =0;
	if(-1 == semctl(semid, 0, SETVAL, arg))						// set the 1th semaphore in set to 0
	{
		fprintf(stderr,"semctl Error:%s\a\n",strerror(errno));
		exit(1);
	}
	value =semctl(semid, 0, GETVAL);
	printf("value is:%d\n", value);
	exit(0);

	/*fork a new process*/
	pid =fork();
	if(0 == pid)		//child process
	{
		sleep(10);
		printf("i'm child!\n");

		child_sops.sem_num	=0;
		child_sops.sem_op	=1;
		child_sops.sem_flg	=0;
		semop(semid, &child_sops, 1);				// returning semaphore's resources
		exit(0);

	}//end of if

	parent_sops.sem_num		=0;
	parent_sops.sem_op		=-1;
	parent_sops.sem_flg		=0;
	semop(semid, &parent_sops, 1);					//

	printf("i'm father!\n");
	exit(0);

	//fprintf(stderr,"Connect Error:%s\a\n",strerror(errno));

}

