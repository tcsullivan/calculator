#ifndef GPIO_H_
#define GPIO_H_

#include <stm32l476xx.h>

#define GPIO_PORT(p, b) GPIO##p, b

enum GPIO_MODE
{
	INPUT = 0,
	OUTPUT,
	ALTERNATE,
	ANALOG
};

enum GPIO_TYPE
{
	PUSHPULL = 0,
	OPENDRAIN
};

enum GPIO_SPEED
{
	LOW = 0,
	MEDIUM,
	HIGH,
	VERYHIGH
};

enum GPIO_PUPD
{
	NOPUPD,
	PULLUP,
	PULLDOWN
};

void gpio_init(void);

void gpio_pupd(GPIO_TypeDef *port, uint8_t pin, uint8_t pupd);
void gpio_type(GPIO_TypeDef *port, uint8_t pin, uint8_t type);
void gpio_speed(GPIO_TypeDef *port, uint8_t pin, uint8_t speed);
void gpio_mode(GPIO_TypeDef *port, uint8_t pin, uint8_t mode);
void gpio_dout(GPIO_TypeDef *port, uint8_t pin, uint8_t val);
uint8_t gpio_din(GPIO_TypeDef *port, uint8_t pin);

#endif // GPIO_H_
