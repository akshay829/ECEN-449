/* All of our linux kernel includes. */
#include <linux/module.h>  /* Needed by all modules */
#include <linux/moduleparam.h>  /* Needed for module parameters */
#include <linux/kernel.h>  /* Needed for printk and KERN_* */
#include <linux/init.h>	   /* Need for __init macros  */
#include <linux/fs.h>	   /* Provides file ops structure */
#include <linux/sched.h>   /* Provides access to the "current" process
task structure */
#include <linux/slab.h> //needed for kmalloc() and kfree()
#include <asm/io.h> //needed for IO reads and writes
#include <asm/uaccess.h>   /* Provides utilities to bring user space
data into kernel space.  Note, it is
processor arch specific. */
#include "xparameters.h" //needed for physical address of the multiplier



/* Some defines */
#define DEVICE_NAME "multiplier"
#define BUF_LEN 80

/* Function prototypes, so we can setup the function pointers for dev
file access correctly. */
int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

/*
* Global variables are declared as static, so are global but only
* accessible within the file.
*/
static int Major;		/* Major number assigned to our device
						driver */
static int Device_Open = 0;	/* Flag to signify open device */

/* This structure defines the function pointers to our functions for
opening, closing, reading and writing the device file.  There are
lots of other pointers in this structure which we are not using,
see the whole definition in linux/fs.h */
static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};
//from xparameters.h
#define PHY_ADDR XPAR_MULTIPLY_0_S00_AXI_BASEADDR //physical address of multiplier
//size of physical address range for multiply
#define MEMSIZE XPAR_MULTIPLY_0_S00_AXI_HIGHADDR - XPAR_MULTIPLY_0_S00_AXI_BASEADDR+1
void* virt_addr; //virtual address pointing to multiplier

				 /* This function is run upon mdoule load. This is where I setup data structs
				 and reserve resources used by the moudle */

				 /* This function is run just prior to the module's removal from the
				 system. I will release ALL resources used by my module here*/

static int __init my_init(void) {
	printk(KERN_INFO "Mapping virutal address...\n");
	//map virtual address to multiplier physical address//use ioremap
	virt_addr = ioremap(PHY_ADDR, MEMSIZE);
	printk("Physical Address: 0x%x\n", PHY_ADDR);
	printk("Virtual Address: 0x%x\n", virt_addr);
	
	/* This function call registers a device and returns a major number
	associated with it.  Be wary, the device file could be accessed
	as soon as you register it, make sure anything you need (ie
	buffers ect) are setup _BEFORE_ you register the device.*/
	Major = register_chrdev(0, DEVICE_NAME, &fops); //dynamic allocation

													/* Negative values indicate a problem */
	if (Major < 0) {
		printk(KERN_ALERT "Registering char device failed with %d\n", Major);
		return Major;
	}

	printk(KERN_INFO "Registered a device with dynamic Major number of %d\n", Major);
	printk(KERN_INFO "Create a device file for this device with this command:\n'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);

	return 0;		/* success */
}

/*
* This function is called when the module is unloaded, it releases
* the device file.
*/
static void __exit my_cleanup(void) {
	// Unregister the device
	unregister_chrdev(Major, DEVICE_NAME);
	printk(KERN_ALERT "unmapping virtual address space...\n");
	iounmap((void*)virt_addr);
}

/*
* Called when a process tries to open the device file, like "cat
* /dev/my_chardev".  Link to this function placed in file operations
* structure for our device file.
*/
static int device_open(struct inode *inode, struct file *file) {
	printk(KERN_ALERT "Device has been opened\n");
	return 0;
}

/*
* Called when a process closes the device file.
*/
static int device_release(struct inode *inode, struct file *file) {
	printk(KERN_ALERT "Device has been closed\n");
	return 0;
}

/*
* Called when a process, which already opened the dev file, attempts
* to read from it.
*/
static ssize_t device_read(struct file *filp, /* see include/linux/fs.h*/
	char *buffer,      /* buffer to fill with
					   data */
	size_t length,     /* length of the
					   buffer  */
	loff_t * offset)
{
	/*
	* Number of bytes actually written to the buffer
	*/
	int bytes_read = 0;
	//allocating kernel buffer (not all may be written to user space though)
	int* kernelBuffer = (int*)kmalloc(length * sizeof(int), GFP_KERNEL);
	kernelBuffer[0] = ioread32(virt_addr);
	kernelBuffer[1] = ioread32(virt_addr + 4);
	kernelBuffer[2] = ioread32(virt_addr + 8);
	char* kernelBuff = (char*)kernelBuffer; //bytes will be written one at a time
	/*
	* Actually put the data into the buffer
	*/
	/* printk("Kernel Buffer[0] = %d\n", kernelBuffer[0]);
	printk("Kernel Buffer[1] = %d\n", kernelBuffer[1]);
	printk("Kernel Buffer[2] = %d\n", kernelBuffer[2]); */ //used for debugging
	int i;
	for (i = 0; i < length; i++) { //a while loop messes everything up
		/*
		* The buffer is in the user data segment, not the kernel segment
		* so "*" assignment won't work.  We have to use put_user which
		* copies data from the kernel data segment to the user data
		* segment.
		*/
		put_user(*(kernelBuff++), buffer++); /* one char at a time... */

		bytes_read++;
	}

	kfree(kernelBuffer);
	/*
	* Most read functions return the number of bytes put into the
	* buffer
	*/
	//printk("bytes_read: %d\n", bytes_read);
	return bytes_read;
}

/*
* Called when a process writes to dev file: echo "hi" > /dev/hello
* Next time we'll make this one do something interesting.
*/
static ssize_t
device_write(struct file *filp, const char *buff, size_t len, loff_t * off) {
	char* kernelBuffer = (char*)kmalloc((len + 1) * sizeof(char), GFP_KERNEL);
	/* First I initlize kernelBuffer as a char* so we can write one
		byte at a time to it from user buffer*/
	int i;
	for (i = 0; i < len; i++) {
		get_user(kernelBuffer[i], buff++);
	}
	
	kernelBuffer[len] = '\0';
	/* Convert kernelBuffer to int* so we can write to
		multiplication peripheral more easily */
	int* intBuffer = (int*)kernelBuffer; 
	
	//write to register 0
	printk(KERN_INFO "Writing %d to register 0\n", intBuffer[0]);
	iowrite32(intBuffer[0], virt_addr + 0); //base address + offset
	
	//write to register 1
	printk(KERN_INFO "Writing %d to register 1\n", intBuffer[1]);
	iowrite32(intBuffer[1], virt_addr + 4);
	
	kfree(intBuffer);
	return i; //number of bytes written
}


/* These define info that can be displayed by modinfo */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Chris Padilla and others");
MODULE_DESCRIPTION("Module which creates a character device and allows user interaction with it");

/* Here we define which functions we want to use for initialization
and cleanup */
module_init(my_init);
module_exit(my_cleanup);
