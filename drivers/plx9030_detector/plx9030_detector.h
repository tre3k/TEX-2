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


/* ============== detector function ============== */
void init_detector(unsigned long portCS0, void __iomem *memCS3);
u16 readFIFOData(void __iomem *memCS3);

/* ============== function of prototypes for PCI ============== */
static int plx_device_probe(struct pci_dev *pdev, const struct pci_device_id *ent);
static void plx_device_remove(struct pci_dev *pdev);

// Vendor ID device 0x10b5 0x90f1 <- plx9030 chip
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




#endif // PLX9030_DETECTOR_H
