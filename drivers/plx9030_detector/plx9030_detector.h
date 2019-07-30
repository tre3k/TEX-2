/* Copyright (c) Petersburg Nuclear Physics Institute 2019 
 * LO, Gatchina Orlova Roscha NRC KI, PNPI 
 * Autros: Kirill Pshenichnyi
 *
 * It is free software
 */


#ifndef PLX9030_DETECTOR_H
#define PLX9030_DETECTOR_H

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/pci.h>
#include <linux/delay.h>

#define DEBUG

#define MAX_DEVICES 8
#define MODULE_NAME "plx9030_detector"
#define DEVICE_FILENAME "plxdetector"

#define ADDR_SN_IN_EEPROP 0x00a00003
#define VPD_DATA 0x50
#define VPD_ADDR 0x4c


/* for user also defined */
#define IOCTL_INIT_DETECTOR 0xfacecafe             // for initialization detector
#define IOCTL_START_DETECTOR IOCTL_INIT_DETECTOR+1 // for start measure
#define IOCTL_STOP_DETECTOR IOCTL_INIT_DETECTOR+2  // for stop measure
#define IOCTL_READ_DETECTOR IOCTL_INIT_DETECTOR+3  // for read data from FIFO
#define IOCTL_TEST_DETECTOR IOCTL_INIT_DETECTOR+4  // for test memory half/full/empty

enum{
  MEMORY_EMPTY,
  MEMORY_FULL,
  MEMORY_HALF
};

/* ********************* */

static unsigned int gCount = 0;
static dev_t tdev;
static unsigned int gMajor;


/* ============== detector function ============== */
static int init_detector(unsigned long portCS0, void __iomem *memCS3);
static u16 readFIFOData(void __iomem *memCS3);
static void start_measure(unsigned long portCS0,void __iomem *memCS3);
static void stop_measure(void __iomem *memCS3);
static u8 test_detector(unsigned long portCS0);

/* ============== function of prototypes for PCI ============== */
static int plx_device_probe(struct pci_dev *pdev, const struct pci_device_id *ent);
static void plx_device_remove(struct pci_dev *pdev);

// Vendor deviceID and vendorID 0x10b5 0x90f1 <- plx9030 chip
static struct pci_device_id plx_ids_table[] =  {
						{PCI_DEVICE(0x10b5,0x90f1)},
						{0,}
};

// pci_driver struct for pci_register_driver();
static struct pci_driver s_pci_driver = {
					      .name = MODULE_NAME,
					      .id_table = plx_ids_table,
					      .probe    = plx_device_probe,
					      .remove   = plx_device_remove
};


/* ============== Char device functions  ============== */
struct my_chrdevice_data{
  struct cdev cdev;
  struct class *dev_class; 

  dev_t mkdev;
  
  unsigned long cs0_port;
  unsigned long cs1_port;
  void __iomem *cs2_mem;
  void __iomem *cs3_mem;

  long lencs0,lencs1,lencs2,lencs3;
  
  unsigned int major;
  unsigned int number;

  unsigned int flag;
  
};

struct my_chrdevice_data devs[MAX_DEVICES];

static int init_chrdev(void);
static void remove_chrdev(void);

static ssize_t device_file_read(struct file *f, char __user *buff, size_t count, loff_t *offset);
static ssize_t device_file_write(struct file *f, const char __user *buff, size_t count, loff_t *offset);
static long device_file_ioctl(struct file *f, unsigned int ioctl_num, unsigned long ioctl_param);
static int device_file_release(struct inode *inode, struct file *f);
static int device_file_open(struct inode *inode, struct file *f);

struct file_operations s_file_operations = {
						  .owner = THIS_MODULE,
						  .open = device_file_open,
						  .release = device_file_release,
						  .read = device_file_read,
						  .write = device_file_write,
						  .unlocked_ioctl = device_file_ioctl
};



#endif // PLX9030_DETECTOR_H
