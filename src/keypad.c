#include <keypad.h>
#include <gpio.h>

#define PIN_0 GPIO_PORT(A, 11)
#define PIN_1 GPIO_PORT(B, 13)
#define PIN_2 GPIO_PORT(B, 2)
#define PIN_3 GPIO_PORT(A, 12)
#define PIN_4 GPIO_PORT(B, 14)
#define PIN_5 GPIO_PORT(B, 11)
#define PIN_6 GPIO_PORT(C, 5)
#define PIN_7 GPIO_PORT(B, 15)
#define PIN_8 GPIO_PORT(B, 12)
#define PIN_9 GPIO_PORT(C, 6)
#define PIN_S GPIO_PORT(B, 1)
#define PIN_P GPIO_PORT(C, 8)

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
	uint16_t keycode;
} key_t;

static const key_t keypad_map[12] = {
	{ PIN_0, K0 },
	{ PIN_1, K1 },
	{ PIN_2, K2 },
	{ PIN_3, K3 },
	{ PIN_4, K4 },
	{ PIN_5, K5 },
	{ PIN_6, K6 },
	{ PIN_7, K7 },
	{ PIN_8, K8 },
	{ PIN_9, K9 },
	{ PIN_S, KS },
	{ PIN_P, KP }
};

void keypad_init(void)
{
	for (uint8_t i = 0; i < 12; i++) {
		GPIO_TypeDef *p = keypad_map[i].port;
		uint16_t pin = keypad_map[i].pin;
		gpio_mode(p, pin, OUTPUT);
		gpio_dout(p, pin, 0);
		gpio_mode(p, pin, INPUT);
		gpio_pupd(p, pin, PULLDOWN);
	}
}

uint16_t keypad_get(void)
{
	uint16_t state = 0;
	for (uint8_t i = 0; i < 12; i++) {
		if (gpio_din(keypad_map[i].port, keypad_map[i].pin))
			state |= keypad_map[i].keycode;
	}
	return state;
}

uint8_t keypad_isdown(uint16_t keycode)
{
	return (keypad_get() & keycode);
}
