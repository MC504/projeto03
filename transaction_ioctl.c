#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <asm/uaccess.h>

#include "transaction_ioctl.h"
 
#define FIRST_MINOR 0
#define MINOR_CNT 1
 
static dev_t dev;
static struct cdev c_dev;
static struct class *cl;

static int code = -1; 
static float value = -1;
static char number[CARDSIZE]="";
static char name[NAMESIZE]="";


char *strcpy(char *s1, const char *s2)
{
    char *s = s1;
    while ((*s++ = *s2++) != 0);
    return (s1);
}
 
static int my_open(struct inode *i, struct file *f)
{
    return 0;
}
static int my_close(struct inode *i, struct file *f)
{
    return 0;
}
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
static int my_ioctl(struct inode *i, struct file *f, unsigned int cmd, unsigned long arg)
#else
static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
#endif
{
    transaction_arg_t t;
 
    switch (cmd)
    {
        case GET_TRANSACTION:
	    if(code<0){
	    	return -EINVAL;
	    }
            t.code = code;
            t.value = value;
            strcpy(t.number, number);
            strcpy(t.name, name);

            if (copy_to_user((transaction_arg_t *)arg, &t, sizeof(transaction_arg_t)))
            {
                return -EACCES;
            }
            break;
        
	case CLR_TRANSACTION:
            code = -1;
            value = -1;
            strcpy(number, "");
            strcpy(name, "");
            break;
        
	case SET_TRANSACTION:
            if (copy_from_user(&t, (transaction_arg_t *)arg, sizeof(transaction_arg_t)))
            {
                return -EACCES;
            }
            code = t.code;
            value = t.value;
            strcpy(number, t.number);
            strcpy(name, t.name);
            break;
        
	default:
            return -EINVAL;
    }
 
    return 0;
}
 
static struct file_operations transaction_fops =
{
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_close,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
    .ioctl = my_ioctl
#else
    .unlocked_ioctl = my_ioctl
#endif
};
 
static int __init transaction_ioctl_init(void)
{
    int ret;
    struct device *dev_ret;
 
 
    if ((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "transaction_ioctl")) < 0)
    {
        return ret;
    }
 
    cdev_init(&c_dev, &transaction_fops);
 
    if ((ret = cdev_add(&c_dev, dev, MINOR_CNT)) < 0)
    {
        return ret;
    }
     
    if (IS_ERR(cl = class_create(THIS_MODULE, "char")))
    {
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(cl);
    }
    if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "transaction")))
    {
        class_destroy(cl);
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(dev_ret);
    }
 
    return 0;
}
 
static void __exit transaction_ioctl_exit(void)
{
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
}
 
module_init(transaction_ioctl_init);
module_exit(transaction_ioctl_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AJM");
MODULE_DESCRIPTION("Transaction ioctl() Char Driver");
