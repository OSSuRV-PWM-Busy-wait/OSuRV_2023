
#ifndef LOG_H
#define LOG_H

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>   
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

typedef struct {
    u64 t;
    // flags.
    u8 state;
    u8 late;
} log_entry_t;


int log__init(void);
void log__exit(void);
void log__add(u64, u8, u8);

#endif


