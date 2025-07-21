#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

char* getline_buf = NULL;
int getline_bufsiz = 0;

char* cdhist_getline(FILE* fh) {
     if (!getline_buf) {
          getline_bufsiz = 4096;
          if (!(getline_buf = malloc(getline_bufsiz))) {
               perror("malloc");
               exit(1);
          }
     }
     char* p = getline_buf;
     int init = 1;
     while (1) {
          int bytes = getline_bufsiz - (p - getline_buf); /* remaining buffer size */
          errno = 0;
          int is_eof = feof(fh);
          char* pp = fgets(p, bytes, fh);
          if (!pp) {
               if (ferror(fh)) {
                    perror("cdhist_getline");
                    exit(errno);
               }
               return NULL;
               return NULL;
          }
          init = 0;
          if (strlen(p) == bytes - 1 && p[bytes - 2] != '\n') {
               int ofs = (p - getline_buf) + bytes - 1;
               if (!(getline_buf = realloc(getline_buf,
                                           getline_bufsiz *= 2))) {
                    perror("realloc");
                    exit(1);
               }
               p = getline_buf + ofs;
               continue;
          }
          return getline_buf;
     }
}
