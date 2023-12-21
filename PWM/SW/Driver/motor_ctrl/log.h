
#ifndef LOG_H
#define LOG_H

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>   
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

static ssize_t read_proc(struct file *filp, char __user *buffer, size_t length, loff_t * offset);
int log_init(void);
void log_exit(void);

#endif


