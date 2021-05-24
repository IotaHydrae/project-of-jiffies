/**********************************************************************
 * 功能描述： 1.父进程从键盘输入字符串到共享内存.
              2.子进程删除字符串中的空格并打印.
              3.父进程输入quit后删除共享内存和信号灯集，程序结束.
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期        版本号     修改人          修改内容
 * -----------------------------------------------
 * 2020/05/16       V1.0      zh(ryan)        创建
 ***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>

#define N 64
#define READ 0
#define WRITE 1

union semun {
	int val;
        struct semid_ds *buf;
        unsigned short *array;
        struct seminfo *__buf;
};

void init_sem(int semid, int s[], int n)
{
	int i;
	union semun myun;

	for (i = 0; i < n; i++){
		myun.val = s[i];
		semctl(semid, i, SETVAL, myun);
	}
}

void pv(int semid, int num, int op)
{
	struct sembuf buf;

	buf.sem_num = num;
	buf.sem_op = op;
	buf.sem_flg = 0;
	semop(semid, &buf, 1);
}

int main(int argc, char *argv[])
{
	int shmid, semid, s[] = {0, 1};
	pid_t pid;
	key_t key;
	char *shmaddr;

	key = ftok(".", 's');
	if (key == -1){
		perror("ftok");
		exit(-1);
	}

	shmid = shmget(key, N, IPC_CREAT|0666);
	if (shmid < 0) {
		perror("shmid");
		exit(-1);
	}

	semid = semget(key, 2, IPC_CREAT|0666);
	if (semid < 0) {
		perror("semget");
		goto __ERROR1;
	}
	init_sem(semid, s, 2);

	shmaddr = shmat(shmid, NULL, 0);
	if (shmaddr == NULL) {
		perror("shmaddr");
		goto __ERROR2;
	}

	pid = fork();
	if(pid < 0) {
		perror("fork");
		goto __ERROR2;
	} else if (pid == 0) {
		char *p, *q;
		while(1) {
			pv(semid, READ, -1);
			p = q = shmaddr;
			while (*q) {
				if (*q != ' ') {
					*p++ = *q;
				}
				q++;
			}
			*p = '\0';
			printf("%s", shmaddr);
			pv(semid, WRITE, 1);
		}
	} else {
		while (1) {
			pv(semid, WRITE, -1);
			printf("input > ");
			fgets(shmaddr, N, stdin);
			if (strcmp(shmaddr, "quit\n") == 0) break;
				pv(semid, READ, 1);
			}
			kill(pid, SIGUSR1);
		}

	__ERROR2:
		semctl(semid, 0, IPC_RMID);
	__ERROR1:
		shmctl(shmid, IPC_RMID, NULL);
		return 0;
}


