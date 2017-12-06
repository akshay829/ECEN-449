#include <linux/module.h> //needed by all modules
#include <linux/kernel.h> //needed for KERN_ and prink
#include <linux/init.h> //needed for __init and __exit macros
#include <asm/io.h> //needed for IO reads and writes
#include "xparameters.h" //needed for physical address of the multiplier

//from xparameters.h
#define PHY_ADDR XPAR_MULTIPLY_0_S00_AXI_BASEADDR //physical address of multiplier
//size of physical address range for multiply
#define MEMSIZE XPAR_MULTIPLY_0_S00_AXI_HIGHADDR - XPAR_MULTIPLY_0_S00_AXI_BASEADDR+1

void* virt_addr; //virtual address pointing to multiplier

/* This function is run upon mdoule load. This is where I setup data structs
	and reserve resources used by the moudle */
static int __init my_init(void) {
	//Linux kernel's version of printf
	printk(KERN_INFO "Mapping virutal address...\n");
	//map virtual address to multiplier physical address//use ioremap
	virt_addr = ioremap(PHY_ADDR, MEMSIZE);
	
	//write 7 to register 0
	printk(KERN_INFO "Writing a 7 to register 0\n");
	iowrite32(7, virt_addr+0); //base address + offset
	//write 2 to register 1
	printk(KERN_INFO "Writing 2 to register 1\n");
	//use iowrite32
	iowrite32(2, virt_addr + 4);
	printk("Read %d from register 0\n", ioread32(virt_addr + 0));
	printk("Read %d from register 1\n", ioread32(virt_addr + 4));
	printk("Read %d from register 2\n", ioread32(virt_addr + 8));
	
	printk("Physical Address: 0x%x\n", PHY_ADDR);
	printk("Virtual Address: 0x%x\n", virt_addr);
	
	//a non 0 return means init_module failed 
	return 0;
}

/* This function is run just prior to the module's removal from the
	system. I will release ALL resources used by my module here*/
	
static void __exit my_exit(void) {
	printk(KERN_ALERT "unmapping virtual address space...\n");
	iounmap((void*)virt_addr);
}

//These define infor that can be displayed by modinfo
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ECEN 449 Student (and others)");
MODULE_DESCRIPTION("Simple Multiplier module");

//here we define which functions we want to use for intialization
//and cleanup

module_init(my_init);
module_exit(my_exit);