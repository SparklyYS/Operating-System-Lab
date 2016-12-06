/*
 * linux/kernel/who.c
 *
 * (C) SparklyYS
 */

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <asm/segment.h>

int buffer_name[100]={0}; //Save the user's name temporarily
int sys_iam(const char* name)
{
	int i = 0;
	int c = 0;
	//check the length
	while(get_fs_byte(name+i)!='\0')
	{
	    if(i > 23) return -EINVAL;
	    i++;
	}
	//memset(buffer_name, 0, sizeof(buffer_name));
	for(i = 0; i < 100; i++)
	{
	    buffer_name[i] = 0;
	}
	i = 0;
	while((c=get_fs_byte(name+i))!='\0')
	{
	    buffer_name[i++] = c;
	}
	return i;
}

int sys_whoami(char* name, unsigned int size)
{
	int i = 0;
	while(buffer_name[i]!='\0')
	{
		if(i >= 23 || i > size-1) //The length is more than 23 or size
			return -EINVAL;
		put_fs_byte(buffer_name[i],(name+i));
		i++;
	}
	return i;
}	
