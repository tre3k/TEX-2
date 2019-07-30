/* Copyright (c) Petersburg Nuclear Physics Institute 2019 
 * LO, Gatchina Orlova Roscha NRC KI, PNPI 
 * Autros: Kirill Pshenichnyi
 *
 * It is free software
 */


#include "plx9030_detector.h"

static int __init init_plx9030_detector(void){
  int retval = 0;
  printk(KERN_INFO MODULE_NAME ": module init\n");

  retval = alloc_chrdev_region(&tdev,0,MAX_DEVICES,"plx9030_detector_dev");
  gMajor = MAJOR(tdev);
  printk(KERN_INFO MODULE_NAME ": char device major: %d\n",gMajor);
  if(retval < 0) return retval;
  
  retval = pci_register_driver(&s_pci_driver);
  
  return retval;
}

static void __exit exit_plx9030_detector(void){
  gCount=0;
  pci_unregister_driver(&s_pci_driver);
  unregister_chrdev_region(tdev,MAX_DEVICES);
  printk(KERN_INFO MODULE_NAME ": module exit\n");
  return;
}


/* ============== PCI Functions ============== */
// pci probe function 
static int plx_device_probe(struct pci_dev *pdev, const struct pci_device_id *ent){
  int retval = 0;

  u16 vendorID,deviceID,revision;
  u32 serialNM = 0;
  char serialstring[9];

  int bar;
  unsigned long addr_start, addr_end, len;

  unsigned long cs0_port;
  unsigned long cs1_port;
  void __iomem *cs2_mem;
  void __iomem *cs3_mem;
  
  retval = pci_enable_device(pdev);
  if(retval < 0){
    printk(KERN_ERR MODULE_NAME ": ERROR! Can't enable pci device.\n");
    return retval;
  }

  retval = pci_request_regions(pdev,MODULE_NAME);
  pci_enable_device_mem(pdev);
  pci_set_master(pdev);
  
  pci_read_config_word(pdev,PCI_VENDOR_ID,&vendorID);
  pci_read_config_word(pdev,PCI_DEVICE_ID,&deviceID);
  pci_read_config_word(pdev,PCI_REVISION_ID,&revision); 
  
  pci_write_config_dword(pdev,VPD_ADDR,ADDR_SN_IN_EEPROP & 0x7fffffff);
  mdelay(100);
  pci_read_config_dword(pdev,VPD_DATA,&serialNM);
  
  /* test on detector */
  if(serialNM==114){  //detector ID serial number
    sprintf(serialstring,"%.8x\x00",serialNM&0xffffffff);
    printk(KERN_INFO MODULE_NAME ": detector found, vendorID: 0x%.4x, deviceID:  0x%.4x\n"
	   MODULE_NAME ": revision: 0x%.4x. Seral number: 0x%.8x (%d)\n",
	 vendorID,deviceID,revision,serialNM&0xffffffff,serialNM);
  }else{
    printk(KERN_INFO MODULE_NAME ": device is not detector\n");
    return retval;   // return if this device not detector
  }

  printk(KERN_INFO MODULE_NAME ": *************** BARs ***************\n");
  for(bar=0; bar<6; bar++){
    addr_start = pci_resource_start(pdev,bar);
    addr_end = pci_resource_end(pdev,bar);
    len = pci_resource_len(pdev,bar);

    if(bar<2){
      printk(KERN_INFO MODULE_NAME ": "
	     "base addr%d: start 0x%x, end: 0x%x, len %d\n",
	     bar,addr_start,addr_end,len);
    }else{
      printk(KERN_INFO MODULE_NAME ": "
	     "base addr%d (CS%d): start 0x%x, end: 0x%x, len %d\n",
	     bar,bar-2,addr_start,addr_end,len);
    }
  }
  
  // get addrs value and port to devs array
  devs[gCount].cs0_port = pci_resource_start(pdev,2);
  devs[gCount].cs1_port = pci_resource_start(pdev,3);
  devs[gCount].cs2_mem = pci_ioremap_bar(pdev,4);
  devs[gCount].cs3_mem = pci_ioremap_bar(pdev,5);

  devs[gCount].lencs0 = pci_resource_len(pdev,2);
  devs[gCount].lencs1 = pci_resource_len(pdev,3);
  devs[gCount].lencs2 = pci_resource_len(pdev,4);
  devs[gCount].lencs3 = pci_resource_len(pdev,5);

  devs[gCount].number = gCount;

  retval = init_chrdev();
  
  gCount++;
  return retval;
}


// pci remove functions
static void plx_device_remove(struct pci_dev *pdev){
  pci_release_regions(pdev);
  pci_disable_device(pdev);
  remove_chrdev();
  printk(KERN_INFO MODULE_NAME ": remove device: %s%d\n",DEVICE_FILENAME,gCount);
  gCount++;
  return;
}


/* ============== Char device functions  ============== */
// read from file
static ssize_t device_file_read(struct file *f, char __user *buff, size_t count, loff_t *offset){

  return 0;
}

// write to file
static ssize_t device_file_write(struct file *f, const char __user *buff, size_t count, loff_t *offset){

  return 0;
}
// ioctl file
static long device_file_ioctl(struct file *f, unsigned int ioctl_num, unsigned long ioctl_param){
  switch(ioctl_num){
  case IOCTL_INIT_DETECTOR:
    
    break;
  }
  return 0;
}

// release file
static int device_file_release(struct inode *inode, struct file *f){

  return 0;
}

// open file
static int device_file_open(struct inode *inode, struct file *f){

  return 0;
}

static int init_chrdev(void){
  int retval = 0;

  char device_name[128];
  char device_name_class[128];

  
  sprintf(device_name,"plxdetector%d",gCount);
  sprintf(device_name_class,"plxdetector%d",gCount);

  devs[gCount].dev_class = class_create(THIS_MODULE,device_name_class);
  if(devs[gCount].dev_class == NULL) goto err_class;
  devs[gCount].mkdev = MKDEV(gMajor,gCount);
  if(device_create(devs[gCount].dev_class,NULL,devs[gCount].mkdev,NULL,device_name)==NULL) goto err_device;
  cdev_init(&devs[gCount].cdev,&s_file_operations);
  devs[gCount].cdev.owner = THIS_MODULE;
  retval = cdev_add(&devs[gCount].cdev,devs[gCount].mkdev,1);
  if(retval < 0) goto err_cdevadd;

  printk(KERN_INFO MODULE_NAME ": register device: %s\n",device_name);
  
  return retval;
  
 err_cdevadd:
  device_destroy(devs[gCount].dev_class,tdev);
 err_device:
  class_destroy(devs[gCount].dev_class);
  printk(KERN_ERR MODULE_NAME ": Error create device!\n");
 err_class:
  printk(KERN_ERR MODULE_NAME ": Error create class for char device!\n");
  return -1;
}

static void remove_chrdev(void){
  cdev_del(&(devs[gCount].cdev));
  device_destroy(devs[gCount].dev_class,devs[gCount].mkdev);
  class_destroy(devs[gCount].dev_class);
  return;
}


/* ============== Detector functions ============== */
// init detector
static void init_detector(unsigned long portCS0, void __iomem *memCS3){
  unsigned short int *si_memCS3 = memCS3;

  printk(KERN_INFO MODULE_NAME ": init detector\n");
  // just zero to CS0+0, CS0+2 and CS3+62 [31 words]  
  
#ifdef DEBUG
  printk(KERN_INFO MODULE_NAME ": remap addr CS3: 0x%.8x\n", si_memCS3);
#endif

  // так нужно будет делать, это безопаснее
  /* 
  outb(0x00,portCS0+0);
  outb(0x00,portCS0+2);
  iowrite16(0x0000,memCS3+31*2);

  // set values 0x40 to CS0+2 and 0x03 to CS3+62
  outb(0x40,portCS0+2);
  iowrite16(0x0003,memCS3+31*2);
  */

  outb(0x00,portCS0+0);
  outb(0x00,portCS0+2);
  si_memCS3[31] = 0x0000;

  outb(0x40,portCS0+2);
  si_memCS3[31] = 0x0003;

  si_memCS3[1] = 0x0007;
  si_memCS3[0] = 0xfc81;
  
  //iowrite16(0x1234,memCS3);

  // read FIFO
#ifdef DEBUG
  printk(KERN_INFO MODULE_NAME ": CS3[256]: 0x%.4x\n", si_memCS3[256]);
  printk(KERN_INFO MODULE_NAME ": ioread: 0x%.4x\n", ioread16(memCS3));
#endif
 
  
  return;
}





module_init(init_plx9030_detector);
module_exit(exit_plx9030_detector);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("tre3k pshcyrill@mail.ru");
MODULE_VERSION("1.0");
