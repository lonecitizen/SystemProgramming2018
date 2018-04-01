#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#include <linux/fs.h>

static char* msg;

int simple_open (struct inode *pinode, struct file *pfile){
  printk(KERN_ALERT "Open deviceJino\n");
  return 0;
}

ssize_t simple_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset){
  printk(KERN_ALERT "Read deviceJino\n");

	printk("msg is : %s\n", msg);
	if(strcmp(msg, "test") == 0){
	  copy_to_user(buffer, "GET TEST", sizeof("GET TEST"));
	}
	else{
	  copy_to_user(buffer, "NOT TEST", sizeof("NOT TEST"));
	}

	kfree(msg);

  return 0;
}

ssize_t simple_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset){
  printk(KERN_ALERT "Write deviceJino\n");

  msg = kmalloc(length, GFP_KERNEL);

  if(copy_from_user(msg, buffer, length)){
	  printk("Write error\n");
		return -1;
	}	
 
	return length;
}

int simple_close (struct inode *pinode, struct file *pfile){
  printk(KERN_ALERT "Release deviceJino\n");
  return 0;
}

struct file_operations fop = {
  .owner = THIS_MODULE,
  .open = simple_open,
  .read = simple_read,
  .write = simple_write,
  .release = simple_close,
};

int __init simple_init(void){
  printk(KERN_ALERT "Init deviceJino\n");
  register_chrdev(240, //Major number 
                 "deviceJino", //Driver name
                 &fop);
  return 0;
}

void __exit simple_exit(void){
  printk(KERN_ALERT "Exit deviceJino\n");
  unregister_chrdev(240, "deviceJino");
}

module_init(simple_init);
module_exit(simple_exit);
