#include <gpio.h>

void gpio_init(void)
{
	// enable clocks
	RCC->AHB2ENR |= 0xFF;
}

void gpio_pupd(GPIO_TypeDef *port, uint8_t pin, uint8_t pupd)
{
	port->PUPDR &= ~(0x03 << (2 * pin));
	port->PUPDR |= pupd << (2 * pin);
}

void gpio_speed(GPIO_TypeDef *port, uint8_t pin, uint8_t speed)
{
	port->OSPEEDR &= ~(0x03 << (2 * pin));
	port->OSPEEDR |= speed << (2 * pin);
}

void gpio_type(GPIO_TypeDef *port, uint8_t pin, uint8_t type)
{
	port->OTYPER &= ~(1 << pin);
	port->OTYPER |= type << pin;
}

void gpio_mode(GPIO_TypeDef *port, uint8_t pin, uint8_t mode)
{
	port->MODER &= ~(0x03 << (2 * pin));
	port->MODER |= mode << (2 * pin);
}

void gpio_dout(GPIO_TypeDef *port, uint8_t pin, uint8_t val)
{
	port->BSRR |= (1 << (val ? pin : pin + 16));
}

uint8_t gpio_din(GPIO_TypeDef *port, uint8_t pin)
{
	return port->IDR & (1 << pin);
}
