#define __LIBRARY__
#include <unistd.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <asm/segment.h>
#include <asm/system.h>
#include <signal.h>
#include <errno.h>

#define LOW_MEM 0x100000
#define MAX_ARG_PAGES 32
int keygroup[27] = {0};

int sys_shmget(int key, size_t size)	
{
	unsigned long  tmp;
/*	int i, j;*/
	if(size > 4096)
		return -EINVAL;

	if(keygroup[key])
		return keygroup[key];

	if(!(tmp = get_free_page()))
		return -ENOMEM;
	
	keygroup[key] = (tmp - 0x100000)/4096;
	
	return keygroup[key];
}

/*qu diao wu yong de */
void *sys_shmat(int shmid )		/*O:int shmid, const void *shmaddr */
{
	unsigned long temp, code_base, data_base, data_limit;
	unsigned long address;		/*luojidizhi*/
	int i;
	data_limit = 0x4000000;
	if(!shmid)
		return -EINVAL;
	/*wu li di zhi*/
	temp = shmid*4096  + 0x100000;

	data_base = get_base(current->ldt[1]);

	data_base += data_limit;		

	address = data_limit;	/* .........*/
	for(i = MAX_ARG_PAGES; i >= 0; i--)
	{	
		data_base -= PAGE_SIZE;
		address -= PAGE_SIZE;
	}
	put_page(temp, data_base);			
	return address;
}
