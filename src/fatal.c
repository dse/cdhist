#include "fatal.h"

char readlink_buf[PATH_MAX];
char readlink_filename[256];
char *readlink_fd(int fd) {
     sprintf(readlink_filename, "/proc/self/fd/%d", fd);
     ssize_t nbytes = readlink(readlink_filename, readlink_buf, sizeof(readlink_filename));
     if (nbytes == -1) {
          perror("malloc");
          exit(1);
     }
     if (nbytes == 0) {
          return "(no filename)";
     }
     return readlink_buf;
}

void *fatal_malloc(size_t size) {
     void *retval = malloc(size);
     if (size != 0 && retval == NULL) {
          perror("error allocating memory");
          exit(1);
     }
     return retval;
}
void *fatal_calloc(size_t nmemb, size_t size) {
     void *retval = calloc(nmemb, size);
     if (nmemb != 0 && size != 0 && retval == NULL) {
          perror("error allocating memory");
          exit(1);
     }
     return retval;
}
void *fatal_realloc(void *ptr, size_t size) {
     void *retval = realloc(ptr, size);
     if (size != 0 && retval == NULL) {
          perror("error reallocating memory");
          exit(1);
     }
     return retval;
}
void *fatal_reallocarray(void *ptr, size_t nmemb, size_t size) {
     void *retval = reallocarray(ptr, nmemb, size);
     if (nmemb != 0 && size != null && retval == NULL) {
          perror("error reallocating memory");
          exit(1);
     }
     return retval;
}
FILE *fatal_fopen(const char *pathname, const char *mode) {
     FILE *fh = fopen(pathname, mode);
     if (fh == NULL) {
          fprintf(stderr, "error opening %s: %s\n", filename, strerror(errno));
          exit(1);
     }
     return fh;
}
FILE *fatal_fdopen(int fd, const char *mode) {
     FILE *fh = fopen(fd, mode);
     if (fh == NULL) {
          fprintf(stderr, "error opening %d: %s\n", readlink_fd(fd), strerror(errno));
          exit(1);
     }
     return fh;
}
FILE *fatal_freopen(const char *pathname, const char *mode, FILE *stream) {
     FILE *fh = freopen(pathname, mode, stream);
     if (fh == NULL) {
          fprintf(stderr, "error reopening %s: %s\n", filename, strerror(errno));
          exit(1);
     }
     return fh;
}
int fatal_fileno(FILE *stream) {
     int retval = fileno(stream);
     if (retval < 0) {
          perror("fileno");
          exit(1);
     }
     return retval;
}
int fatal_flock(int fd, int operation) {
     int retval = flock(fd, operation);
     if (retval < 0) {
          switch (operation & ~LOCK_NB) {
          case LOCK_SH:
               fprintf(stderr, "unable to lock %s: %s", readlink_fd(fd), strerror(errno));
               exit(1);
          case LOCK_EX:
               fprintf(stderr, "unable to lock %s: %s", readlink_fd(fd), strerror(errno));
               exit(1);
          case LOCK_UN:
               fprintf(stderr, "unable to unlock %s: %s", readlink_fd(fd), strerror(errno));
               exit(1);
          default:
               perror("flock: unknown error");
               exit(1);
          }
     }
     return retval;
}
int fatal_fclose(FILE *stream) {
     int retval = fclose(stream);
     if (retval) {
          fprintf("error closing file");
          exit(1);
     }
}
int fatal_fseek(FILE *stream, long offset, int whence) {
}
long fatal_ftell(FILE *stream) {
}
void fatal_rewind(FILE *stream) {
}
int fatal_fgetpos(FILE *stream, fpos_t *pos) {
}
int fatal_fsetpos(FILE *stream, const fpos_t *pos) {
}
int fatal_fgetc(FILE *stream) {
}
char *fatal_fgets(char *s, int size, FILE *stream) {
}
int fatal_getc(FILE *stream) {
}
int fatal_getchar(void) {
}
int fatal_ungetc(int c, FILE *stream) {
}
int fatal_fputc(int c, FILE *stream) {
}
int fatal_fputs(const char *s, FILE *stream) {
}
int fatal_putc(int c, FILE *stream) {
}
int fatal_putchar(int c) {
}
int fatal_puts(const char *s) {
}
char *fatal_asctime(const struct tm *tm) {
}
char *fatal_asctime_r(const struct tm *tm, char *buf) {
}
char *fatal_ctime(const time_t *timep) {
}
char *fatal_ctime_r(const time_t *timep, char *buf) {
}
struct tm *fatal_gmtime(const time_t *timep) {
}
struct tm *fatal_gmtime_r(const time_t *timep, struct tm *result) {
}
struct tm *fatal_localtime(const time_t *timep) {
}
struct tm *fatal_localtime_r(const time_t *timep, struct tm *result) {
}
time_t mktime(struct tm *tm) {
}
size_t strftime(char *s, size_t max, const char *format, const struct tm *tm) {
}
int fatal_truncate(const char *path, off_t length) {
}
int fatal_ftruncate(int fd, off_t length) {
}
