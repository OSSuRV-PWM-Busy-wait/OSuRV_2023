
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

typedef struct {
    u64  t;
    // flags.
    u8 state;
    u8 late;
} log_entry_t;

static u32 entry_index = 0;
static log_entry_t log_entries[N_ENTRIES];

static struct proc_dir_entry *proc_file;

static ssize_t read_proc(struct file *filp, char __user *buffer, size_t length, loff_t *ppos)
{
    char buf[BUFFER_SIZE];
    int len = 0;

    printk(KERN_INFO DEV_NAME": read_proc handler");

    if(*ppos > 0 || length < BUFFER_SIZE) {
        return 0;
    }

    len += sprintf(buf, "test\n");
    if(copy_to_user(buffer, buf, len)) {
        return -EFAULT;
    }

    *ppos = len;

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

    printk(KERN_INFO DEV_NAME": Created proc entry.");
    return 0;
}

void log__exit(void) {
    proc_remove(proc_file);
    printk(KERN_INFO DEV_NAME": Removed proc entry.");
}

/*
   void log_add(ns_t t, u8 state, u8 late) {
   log_entries[last_entry].t = t;
   log_entries[last_entry].state = state;
   log_entries[last_entry].late = late;
   last_entry++;
   }
   */

/*
   size_t print_ns_t(const char* buf, ns_t t) {
//TODO %ds%3dm%3du%3dn
//snprintf
}

#define TICKS_NS 100000
u32 ns_t_2_ticks(ns_t t) {
return t/TICKS_NS
}

size_t print_ns_t_and_ticks(const char* buf, ns_t t) {
u32 ticks = ns_t_2_ticks(t);
print_ns_t(buf,
}

void log_print() {
// ns_t and ticks
// rel abs from start
//	rel to prev
// state
// late
}
*/
