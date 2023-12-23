
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
static ktime_t ns_time = 0;

static struct proc_dir_entry *proc_file;


void log__add(u64 t, u8 state, u8 late) {
    log_entries[write_index].t = t - ns_time;
    log_entries[write_index].state = state;
    log_entries[write_index].late = late;
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
    
    len += sprintf(output_buffer, "t %llu\nstate %d\nlate %d\n", log_entries[i].t, log_entries[i].state, log_entries[i].late);
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

    ns_time = ktime_get_ns();
    printk(KERN_INFO DEV_NAME": Created proc entry. Time: %lld", ns_time);
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
