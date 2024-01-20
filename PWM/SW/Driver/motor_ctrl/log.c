
#include "log.h"
#include "include/motor_ctrl.h"
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>         // kmalloc()
#include <linux/uaccess.h>      // copy_to/from_user()
#include <linux/proc_fs.h>

#define N_ENTRIES 1000
#define BUFFER_SIZE 256

static u16 write_index = 0;
static log_entry_t log_entries[N_ENTRIES] = {0};
static struct proc_dir_entry *proc_file;

/*
 * @t - vreme
 * @state - state
 */
void log__add(int64_t t, u8 state, u8 ch) {
	log_entries[write_index].t = t;
	log_entries[write_index].state = state;
	log_entries[write_index].ch = ch;

	write_index = (write_index + 1) % N_ENTRIES;
}

static ssize_t read_proc(struct file *filp, char __user *user_buffer, size_t length, loff_t *ppos)
{
	char output_buffer[BUFFER_SIZE] = {'\0'};
	int len = 0;
	static int i = 0;

	if(i >= N_ENTRIES) {
		i = 0;
		return 0;
	}

	len += sprintf(output_buffer, "%lld %d %d\n", log_entries[i].t, log_entries[i].state, log_entries[i].ch);
	i++;

	if(copy_to_user(user_buffer, output_buffer, len)) {
		return -EFAULT;
	}

	return len;
}

static struct proc_ops proc_fops = {
	.proc_read = read_proc,
};

int log__init(void) {
	proc_file = proc_create("motor_ctrl_log", 0555, NULL, &proc_fops);

	if(proc_file == NULL) {
		printk(KERN_WARNING DEV_NAME": Could not create proc entry.");
		return -1;
	}

	return 0;
}

void log__exit(void) {
	proc_remove(proc_file);
	printk(KERN_INFO DEV_NAME": Removed proc entry.");
}


