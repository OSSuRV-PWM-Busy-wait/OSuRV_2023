
#ifndef LOG_H
#define LOG_H

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>   
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

int log__init(void);
void log__exit(void);

#endif


