#define __LIBRARY__
#include <unistd.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <asm/segment.h>
#include <asm/system.h>
#include <signal.h>

#define NR_SEMS 20

sem_t *array[20]={};
char tname[20]={'\0'};
char dname[20]={'\0'};
char sname[20][20] = {'\0'};
sem_t sem_array[20];


void MakeInit( fifo * q)	/*qing kong fifo*/
{
    q->sem_fd[0] = 0;
    q->sem_fd[1] =  9;
}


int plus(int num)		/*move on*/
{
    return ((num + 1) % 10);
}


struct task_struct * cfd( fifo * q )		/* read data */
{
    int r;
    if( plus(q->sem_fd[1]) == q->sem_fd[0]  )
    {
                return NULL;
    }
    else
    {
   	r = q->sem_fd[0];
        q->sem_fd[0] = plus( q->sem_fd[0] );
        return q->wait[r];
    }
}

void pfd( struct task_struct * child, fifo * q)	/* write data */
{
    if( plus( plus(q->sem_fd[1])) != q->sem_fd[0] )
    {
        q->sem_fd[1] = plus( q->sem_fd[1]);
        q->wait[q->sem_fd[1]] = child;
    }
}


/*...............wake up consumer............................*/
extern void wake_on_sem(struct task_struct *p)
{
      if(p != NULL)
            (*p).state = TASK_RUNNING;
}

int sys_sem_open(const char *name, unsigned int value)
{
    int i = 0,j,k;
    int flag = 0;
    char temp;

    while((temp = get_fs_byte(name+i))!='\0')
    {
        tname[i] = temp;
        i++;
    }
    tname[i] = '\0';
    for (i = 0;i < NR_SEMS; i++)
    {
        flag = strcmp(tname,sname[i]);
        if (!flag)
        {
            array[i] = &sem_array[i];
            return array[i];
        }
    }
    for (i = 0; i < NR_SEMS; i++)
    {
        if (sem_array[i].used==0)
        {
            array[i] = &sem_array[i];
            strcpy(sname[i],tname);
            array[i]->value = value;
            array[i]->used = 1;
            MakeInit( &(array[i]->waitsem));
            return array[i];
        }
    }
    return -1;
}

int sys_sem_wait(sem_t* sem)
{
    cli();
    sem->value--;
    if(sem->value < 0)
    {
        pfd( current, &(sem->waitsem));
        current->state=TASK_UNINTERRUPTIBLE;
        schedule();
		sti();
    }
    sti();
    return 0;
}

int sys_sem_post(sem_t * sem)
{
    cli();
    sem->value++;
    if(sem->value<=0)
    {
       wake_on_sem(cfd(&(sem->waitsem)));
    }
    sti();
    return 0;
}

int sys_sem_unlink(const char *name)
{
    int i = 0;
    int j;
    int flag;
    char temp;


    while((temp = get_fs_byte(&name[i]))!='\0')
    {
        dname[i] = temp;
        i++;
    }
    dname[i] = '\0';
    
    for(i = 0;i < NR_SEMS; i++)
    {
        flag =strcmp(sname[i],dname);
        if(!flag)
        {
            sem_array[i].used = 0;
            sem_array[i].value = 0;
            MakeInit(&(sem_array[i].waitsem));
           for(j = 0; j<20; j++)
            sname[i][j] = '\0';
            return 0;
        }
    }
    return -1;
}


