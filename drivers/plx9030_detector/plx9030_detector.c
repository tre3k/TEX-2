/* Copyright (c) Petersburg Nuclear Physics Institute 2019 
 * LO, Gatchina Orlova Roscha NRC KI, PNPI 
 * Autros: Kirill Pshenichnyi
 *
 * It is free software
 */


#include "plx9030_detector.h"

static int __init init_plx9030_detector(void){

  return 0;
}

static void __exit exit_plx9030_detector(void){

  return;
}

module_init(init_plx9030_detector);
module_exit(exit_plx9030_detector);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kirill Pshenichnyi aka tre3k pshcyrill@mail.ru");
MODULE_VERSION("1.0");
