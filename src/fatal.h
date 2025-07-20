#ifndef FATAL_H
#define FATAL_H

void *fatal_malloc(size_t size);
void fatal_free(void *ptr);
void *fatal_calloc(size_t nmemb, size_t size);
void *fatal_realloc(void *ptr, size_t size);
void *fatal_reallocarray(void *ptr, size_t nmemb, size_t size);
char *fatal_strcpy(char *dest, const char *src);
char *fatal_strcat(char *dest, const char *src);
FILE *fatal_fopen(const char *pathname, const char *mode);
FILE *fatal_fopen(const char *pathname, const char *mode);
FILE *fatal_fdopen(int fd, const char *mode);
FILE *fatal_freopen(const char *pathname, const char *mode, FILE *stream);
int fatal_feof(FILE *stream);
int fatal_ferror(FILE *stream);
int fatal_fileno(FILE *stream);
int fatal_flock(int fd, int operation);
int fatal_fclose(FILE *stream);
int fatal_fseek(FILE *stream, long offset, int whence);
long fatal_ftell(FILE *stream);
void fatal_rewind(FILE *stream);
int fatal_fgetpos(FILE *stream, fpos_t *pos);
int fatal_fsetpos(FILE *stream, const fpos_t *pos);
int fatal_fgetc(FILE *stream);
char *fatal_fgets(char *s, int size, FILE *stream);
int fatal_getc(FILE *stream);
int fatal_getchar(void);
int fatal_ungetc(int c, FILE *stream);
int fatal_fputc(int c, FILE *stream);
int fatal_fputs(const char *s, FILE *stream);
int fatal_putc(int c, FILE *stream);
int fatal_putchar(int c);
int fatal_puts(const char *s);
char *fatal_asctime(const struct tm *tm);
char *fatal_asctime_r(const struct tm *tm, char *buf);
char *fatal_ctime(const time_t *timep);
char *fatal_ctime_r(const time_t *timep, char *buf);
struct tm *fatal_gmtime(const time_t *timep);
struct tm *fatal_gmtime_r(const time_t *timep, struct tm *result);
struct tm *fatal_localtime(const time_t *timep);
struct tm *fatal_localtime_r(const time_t *timep, struct tm *result);
time_t fatal_mktime(struct tm *tm);
size_t fatal_strftime(char *s, size_t max, const char *format, const struct tm *tm);
int fatal_truncate(const char *path, off_t length);
int fatal_ftruncate(int fd, off_t length);

#endif
