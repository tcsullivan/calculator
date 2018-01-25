#ifndef SERIAL_H_
#define SERIAL_H_

void serial_init(void);
void serial_put(int c);

char serial_get(void);
void serial_gets(char *buf, int max);

#endif // SERIAL_H_
