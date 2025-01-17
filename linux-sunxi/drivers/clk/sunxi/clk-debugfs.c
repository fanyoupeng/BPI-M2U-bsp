/*
 * Copyright (C) 2013 Allwinnertech, kevin.z.m <kevin@allwinnertech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/debugfs.h>
#include <asm/uaccess.h>
#include <linux/clk-private.h>
#include <linux/clk-provider.h>
#include <linux/clkdev.h>
#include <linux/clk/sunxi.h>
#include "clk-factors.h"
#include "clk-periph.h"

static struct dentry *my_ccudbg_root;
static int __init debugfs_test_init(void);
extern void __iomem * sunxi_clk_base;
extern void __iomem *sunxi_clk_cpus_base;
extern int sunxi_clk_maxreg;
extern int cpus_clk_maxreg;

struct testclk_data{
	char command[32];
	char name[32];
	char start[32];
	char param[32];
	char info[128];
	char tmpbuf[256];
};
static struct testclk_data testclk_priv;
static void clktest_reg_dump(void)
{
	unsigned int i;
	char dumpline[256];
	printk("Dump Base 0x%lx Regs %d as Follow:\n",(unsigned long __force)sunxi_clk_base, (unsigned int __force)sunxi_clk_maxreg);
	for(i=0;i<=sunxi_clk_maxreg;)
	{
		if(i+12 <= sunxi_clk_maxreg)
			sprintf(dumpline,"[Reg %08x] %08x %08x %08x %08x ",i
					,*((unsigned int*)((char*)sunxi_clk_base +i))
					,*((unsigned int*)((char*)sunxi_clk_base +i+4))
					,*((unsigned int*)((char*)sunxi_clk_base +i+8))
					,*((unsigned int*)((char*)sunxi_clk_base +i+12)));
		else if(i+8 <= sunxi_clk_maxreg)
			sprintf(dumpline,"[Reg %08x] %08x %08x %08x ",i
						,*((unsigned int*)((char*)sunxi_clk_base +i))
						,*((unsigned int*)((char*)sunxi_clk_base +i+4))
						,*((unsigned int*)((char*)sunxi_clk_base +i+8)));
		else if(i+4 <= sunxi_clk_maxreg)
			sprintf(dumpline,"[Reg %08x] %08x %08x ",i
						,*((unsigned int*)((char*)sunxi_clk_base +i))
						,*((unsigned int*)((char*)sunxi_clk_base +i+4)));
		else
			sprintf(dumpline,"[Reg %08x] %08x ",i
						,*((unsigned int*)((char*)sunxi_clk_base +i)));
		printk("%s\n",dumpline);
		i = i+16;
	}
	printk("\n");

	if(cpus_clk_maxreg)
	{
		printk("Dump CPUS Base 0x%lx Regs %d as Follow:\n",(unsigned long __force)sunxi_clk_cpus_base,(unsigned int)cpus_clk_maxreg);
		for(i=0;i<=cpus_clk_maxreg;)
		{
			if(i+12 <= cpus_clk_maxreg)
				sprintf(dumpline,"[Reg %08x] %08x %08x %08x %08x ",i
						,*((unsigned int*)((char*)sunxi_clk_base +i))
						,*((unsigned int*)((char*)sunxi_clk_base +i+4))
						,*((unsigned int*)((char*)sunxi_clk_base +i+8))
						,*((unsigned int*)((char*)sunxi_clk_base +i+12)));
			else if(i+8 <= cpus_clk_maxreg)
				sprintf(dumpline,"[Reg %08x] %08x %08x %08x ",i
							,*((unsigned int*)((char*)sunxi_clk_base +i))
							,*((unsigned int*)((char*)sunxi_clk_base +i+4))
							,*((unsigned int*)((char*)sunxi_clk_base +i+8)));
			else if(i+4 <= cpus_clk_maxreg)
				sprintf(dumpline,"[Reg %08x] %08x %08x ",i
							,*((unsigned int*)((char*)sunxi_clk_base +i))
							,*((unsigned int*)((char*)sunxi_clk_base +i+4)));
			else
				sprintf(dumpline,"[Reg %08x] %08x ",i
							,*((unsigned int*)((char*)sunxi_clk_base +i)));
			printk("%s\n",dumpline);					
			i = i+16;
		}
	}
	printk("\n");	
}
static void clktest_process(void)
{
	int i,j,enabled,command = 0xff;
	unsigned long rate;
	struct clk	*cur_clk=NULL;
	struct clk	*parent_clk=NULL;
	int start = simple_strtoul(testclk_priv.start,NULL,0);
	if(start ==1)
	{
		if(!strcmp(testclk_priv.command,"getparents"))
			command = 1;
		else if(!strcmp(testclk_priv.command,"getparent"))
			command = 2;
		else if(!strcmp(testclk_priv.command,"setparent"))
			command = 3;
		else if(!strcmp(testclk_priv.command,"getrate"))
			command = 4;
		else if(!strcmp(testclk_priv.command,"setrate"))
			command = 5;
		else if(!strcmp(testclk_priv.command,"is_enabled"))
			command = 6;
		else if(!strcmp(testclk_priv.command,"disable"))
			command = 7;
		else if(!strcmp(testclk_priv.command,"enable"))
			command = 8;
		else if(!strcmp(testclk_priv.command,"dumpreg"))
			command = 9;
		else if(!strcmp(testclk_priv.command,"assert"))
			command = 10;
		else if(!strcmp(testclk_priv.command,"deassert"))
			command = 11;
		else
		{
			printk("Error Not support command %s\n",testclk_priv.command);
			command = 0xff;
		}
		if((command != 0xff) && (command != 9) )
		{
			cur_clk = clk_get(NULL,testclk_priv.name);
			if(!cur_clk || IS_ERR(cur_clk))
			{
				cur_clk = NULL;
				printk("Error Found clk %s\n",testclk_priv.name);
				strcpy(testclk_priv.info,"Error");
				return;
			}
			switch(command)
			{
				case 1://getparents
				{
					j =0;
					memset(testclk_priv.info,0x0,sizeof(testclk_priv.info));
					for(i=0;i<cur_clk->num_parents;i++)
					{
						memcpy(&testclk_priv.info[j],cur_clk->parent_names[i],strlen(cur_clk->parent_names[i]));
						j+=strlen(cur_clk->parent_names[i]);
						testclk_priv.info[j] = ' ';
						j++;
					}	
					testclk_priv.info[j]=0;
					break;
				}
				case 2:	//getparent
				{

					parent_clk = clk_get_parent(cur_clk);
					if(!parent_clk || IS_ERR(parent_clk))
					{
						printk("Error Found parent of %s\n",cur_clk->name);
						strcpy(testclk_priv.info,"Error");
					}	
						else
						 strcpy(testclk_priv.info,parent_clk->name);
					break;
				}
				case 3:	//setparent
				{
					if(cur_clk->parent)
					{
						parent_clk = clk_get(NULL,testclk_priv.param);
						if(!parent_clk || IS_ERR(parent_clk))
						{
							printk("Error Found parent %s\n",testclk_priv.param);
							strcpy(testclk_priv.info,"Error");
						}
						else
						{
							clk_set_parent(cur_clk,parent_clk);
							strcpy(testclk_priv.info,cur_clk->parent->name);
							clk_put(parent_clk);
						}
					}
						else
							strcpy(testclk_priv.info,"Error");
						break;
				}
				case 4://getrate
				{
					rate = clk_get_rate(cur_clk);
					if(rate)

						sprintf(testclk_priv.info,"%d",(unsigned int)rate);
					else
						strcpy(testclk_priv.info,"Error");
					break;
				}
				case 5://setrate
				{
					rate = simple_strtoul(testclk_priv.param,NULL,0);
					if(rate)
					{
							clk_set_rate(cur_clk,rate);
							sprintf(testclk_priv.info,"%d",(unsigned int)rate);
					}	
					else
						strcpy(testclk_priv.info,"Error");
					break;
				}
				case 6://is_enabled
				{
					if (!cur_clk->ops->is_enabled)
							enabled = cur_clk->enable_count ? 1 : 0;
					else
							enabled = cur_clk->ops->is_enabled(cur_clk->hw);
					if(enabled)
							strcpy(testclk_priv.info,"enabled");																											
					else
							strcpy(testclk_priv.info,"disabled");
						break;
				}
				case 7://disable
				{
					if (!cur_clk->ops->is_enabled)
						enabled = cur_clk->enable_count ? 1 : 0;
					else
						enabled = cur_clk->ops->is_enabled(cur_clk->hw);
					if(enabled)
						clk_disable_unprepare(cur_clk);
					strcpy(testclk_priv.info,"disabled");
					break;
				}
				case 8://enable
				{
					if (!clk_prepare_enable(cur_clk))
						strcpy(testclk_priv.info,
								"enabled");
					else
						strcpy(testclk_priv.info,
							"enable_failed");
					break;
				}
				case 10://assert
				{
					if(!sunxi_periph_reset_assert(cur_clk))
						strcpy(testclk_priv.info,"asserted");
					else
						strcpy(testclk_priv.info,"failed");
					break;
				}
				case 11://deassert
				{
					if(!sunxi_periph_reset_deassert(cur_clk))
						strcpy(testclk_priv.info,"deasserted");
					else
						strcpy(testclk_priv.info,"failed");
					break;
				}
				default:
					break;
			}
			if(cur_clk)
				clk_put(cur_clk);	
		}
		else if( command == 9)
		{
			clktest_reg_dump();
			strcpy(testclk_priv.info,"OK");
		}
	}
}
//##########command###############
static int ccudbg_command_open(struct inode * inode, struct file * file)
{
	return 0;
}
static int ccudbg_command_release(struct inode * inode, struct file * file)
{
	return 0;
}
static ssize_t ccudbg_command_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{  
	int len	= strlen(testclk_priv.command);
	strcpy(testclk_priv.tmpbuf,testclk_priv.command);
	testclk_priv.tmpbuf[len]=0x0A;
	testclk_priv.tmpbuf[len+1]=0x0;
	len = strlen(testclk_priv.tmpbuf);
	if(len)
	{
		if(*ppos >=len)
			return 0;
		if(count >=len)
			count = len;
		if(count > (len - *ppos))
			count = (len - *ppos);
		if(copy_to_user((void __user *)buf,(const void *)testclk_priv.tmpbuf,(unsigned long)len))
			return -EFAULT;
		*ppos += count;
	}
	else
		count = 0;
	return count;
}

static ssize_t ccudbg_command_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{       
	if( count >= sizeof(testclk_priv.command) )
		return 0;
	if(copy_from_user(testclk_priv.command, buf, count))
		return -EFAULT;
	if(testclk_priv.command[count-1]==0x0A)
		testclk_priv.command[count-1]=0;
	else
		testclk_priv.command[count]=0;
	return count;
}
//##########name###############
static int ccudbg_name_open(struct inode * inode, struct file * file)
{
	return 0;
}
static int ccudbg_name_release(struct inode * inode, struct file * file)
{
	return 0;
}
static ssize_t ccudbg_name_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{    
	int len	= strlen(testclk_priv.name);
	strcpy(testclk_priv.tmpbuf,testclk_priv.name);
	testclk_priv.tmpbuf[len]=0x0A;
	testclk_priv.tmpbuf[len+1]=0x0;
	len = strlen(testclk_priv.tmpbuf);
	if(len)
	{
		if(*ppos >=len)
			return 0;
		if(count >=len)
			count = len;
		if(count > (len - *ppos))
			count = (len - *ppos);
		if(copy_to_user((void __user *)buf,(const void *)testclk_priv.tmpbuf,(unsigned long)len))
			return -EFAULT;
		*ppos += count;
	}
	else
		count = 0;
	return count;
}

static ssize_t ccudbg_name_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{       
	if( count >= sizeof(testclk_priv.name) )
		return 0;
	if(copy_from_user(testclk_priv.name, buf, count))
		return -EFAULT;
	if(testclk_priv.name[count-1]==0x0A)
		testclk_priv.name[count-1]=0;
	else
		testclk_priv.name[count]=0;
	return count;
}
//##########start###############
static int ccudbg_start_open(struct inode * inode, struct file * file)
{
	return 0;
}
static int ccudbg_start_release(struct inode * inode, struct file * file)
{
	return 0;
}
static ssize_t ccudbg_start_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{    
	int len = strlen(testclk_priv.start);
	strcpy(testclk_priv.tmpbuf,testclk_priv.start);
	testclk_priv.tmpbuf[len]=0x0A;
	testclk_priv.tmpbuf[len+1]=0x0;
	len = strlen(testclk_priv.tmpbuf);
	if(len)
	{
		if(*ppos >=len)
			return 0;
		if(count >=len)
			count = len;
		if(count > (len - *ppos))
			count = (len - *ppos);
		if(copy_to_user((void __user *)buf,(const void *)testclk_priv.tmpbuf,(unsigned long)len))
			return -EFAULT;
		*ppos += count;
	}
	else
		count = 0;
	return count;
}
static ssize_t ccudbg_start_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{     
	if( count >= sizeof(testclk_priv.start) )
		return 0;
	if(copy_from_user(testclk_priv.start, buf, count))
		return -EFAULT;
	if(testclk_priv.start[count-1]==0x0A)
		testclk_priv.start[count-1]=0;
	else
		testclk_priv.start[count]=0;
	clktest_process();
	return count;
}
//##########param###############
static int ccudbg_param_open(struct inode * inode, struct file * file)
{
	return 0;
}
static int ccudbg_param_release(struct inode * inode, struct file * file)
{
	return 0;
}
static ssize_t ccudbg_param_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{    
	int len = strlen(testclk_priv.param);
	strcpy(testclk_priv.tmpbuf,testclk_priv.param);
	testclk_priv.tmpbuf[len]=0x0A;
	testclk_priv.tmpbuf[len+1]=0x0;
	len = strlen(testclk_priv.tmpbuf);
	if(len)
	{
		if(*ppos >=len)
			return 0;
		if(count >=len)
			count = len;
		if(count > (len - *ppos))
			count = (len - *ppos);
		if(copy_to_user((void __user *)buf,(const void *)testclk_priv.tmpbuf,(unsigned long)len))
			return -EFAULT;
		*ppos += count;
	}
	else
		count = 0;
	return count;
}
static ssize_t ccudbg_param_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{       
	if( count >= sizeof(testclk_priv.param) )
		return 0;
	if(copy_from_user(testclk_priv.param, buf, count))
		return -EFAULT;
	if(testclk_priv.param[count-1]==0x0A)
		testclk_priv.param[count-1]=0;
	else
		testclk_priv.param[count]=0;
	return count;
}
//##########info###############
static int ccudbg_info_open(struct inode * inode, struct file * file)
{
	return 0;
}
static int ccudbg_info_release(struct inode * inode, struct file * file)
{
	return 0;
}
static ssize_t ccudbg_info_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{    
	int len	= strlen(testclk_priv.info);
	strcpy(testclk_priv.tmpbuf,testclk_priv.info);
	testclk_priv.tmpbuf[len]=0x0A;
	testclk_priv.tmpbuf[len+1]=0x0;
	len	= strlen(testclk_priv.tmpbuf);
	if(len)
	{
		if(*ppos >=len)
			return 0;
		if(count >=len)
			count = len;
		if(count > (len - *ppos))
			count = (len - *ppos);
		if(copy_to_user((void __user *)buf,(const void *)testclk_priv.tmpbuf,(unsigned long)len))
			return -EFAULT;
		*ppos += count;
	}
	else
		count = 0;
	return count;
}
static ssize_t ccudbg_info_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{       
	if( count >= sizeof(testclk_priv.info) )
		return 0;
	if(copy_from_user(testclk_priv.info, buf, count))
		return -EFAULT;
	if(testclk_priv.info[count-1]==0x0A)
		testclk_priv.info[count-1]=0;
	else
		testclk_priv.info[count]=0;
	return count;
}
static const struct file_operations command_ops = {
	.write      = ccudbg_command_write,
	.read       = ccudbg_command_read,
	.open       = ccudbg_command_open,
	.release    = ccudbg_command_release,
};
static const struct file_operations name_ops = {
	.write      = ccudbg_name_write,
	.read       = ccudbg_name_read,
	.open       = ccudbg_name_open,
	.release    = ccudbg_name_release,
};
static const struct file_operations start_ops = {
	.write      = ccudbg_start_write,
	.read       = ccudbg_start_read,
	.open       = ccudbg_start_open,
	.release    = ccudbg_start_release,
};
static const struct file_operations param_ops = {
	.write      = ccudbg_param_write,
	.read       = ccudbg_param_read,
	.open       = ccudbg_param_open,
	.release    = ccudbg_param_release,
};
static const struct file_operations info_ops = {
	.write      = ccudbg_info_write,
	.read       = ccudbg_info_read,
	.open       = ccudbg_info_open,
	.release    = ccudbg_info_release,
};
static int __init debugfs_test_init(void)
{
	my_ccudbg_root = debugfs_create_dir("ccudbg", NULL);
	if(!debugfs_create_file("command", 0644, my_ccudbg_root, NULL,&command_ops))
		goto Fail;
	if(!debugfs_create_file("name", 0644, my_ccudbg_root, NULL,&name_ops))
		goto Fail;
	if(!debugfs_create_file("start", 0644, my_ccudbg_root, NULL,&start_ops))
		goto Fail;	
	if(!debugfs_create_file("param", 0644, my_ccudbg_root, NULL,&param_ops))
		goto Fail;	
	if(!debugfs_create_file("info", 0644, my_ccudbg_root, NULL,&info_ops))
		goto Fail;	
	return 0;

	Fail:
	debugfs_remove_recursive(my_ccudbg_root);
	my_ccudbg_root = NULL;
	return -ENOENT;
}

late_initcall(debugfs_test_init);

