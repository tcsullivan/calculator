#ifndef FLASH_H_
#define FLASH_H_

#include <stdint.h>

void flash_init(void);

void flash_read(char *buf, uint32_t addr, unsigned int count);
void flash_write(const char *buf, uint32_t addr, unsigned int count);

#endif // FLASH_H_
