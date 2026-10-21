#ifndef REG_ACCESS_H
#define REG_ACCESS_H

#include <stdint.h>

/*Turns an addres into a readable/writable 32-bit register*/
#define REG32(addr)     (*(volatile uint32_t *) (addr))

#endif