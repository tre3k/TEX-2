/* Copyright (c) Petersburg Nuclear Physics Institute 2019 
 * LO, Gatchina Orlova Roscha NRC KI, PNPI 
 * Autros: Kirill Pshenichnyi
 *
 * It is free software
 */


#include "plx9030_detector.h"

static int __init init_plx9030_detector(void){
  printk(KERN_INFO MODULE_NAME ": module init\n");

  pci_register_driver(&s_pci_driver);
  
  return 0;
}

static void __exit exit_plx9030_detector(void){
  gCount=0;
  pci_unregister_driver(&s_pci_driver);
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

  gCount++;
  return retval;
}


// pci remove functions
static void plx_device_remove(struct pci_dev *pdev){
  pci_release_regions(pdev);
  pci_disable_device(pdev);

  return;
}


/* ============== Char device functions  ============== */

/* ============== Detector functions ============== */

module_init(init_plx9030_detector);
module_exit(exit_plx9030_detector);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("tre3k pshcyrill@mail.ru");
MODULE_VERSION("1.0");
