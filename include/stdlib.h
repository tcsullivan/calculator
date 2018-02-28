#ifndef STDLIB_H_
#define STDLIB_H_

char *snprintf(char *buf, unsigned int max, const char *format, ...);
float strtof(const char *s, char **endptr);
int atoi(const char *s);

#endif // STDLIB_H_
