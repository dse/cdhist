#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

char* getline_buf = NULL;
int getline_bufsiz = 0;

char* cdhist_getline(FILE* fp) {
     if (!getline_buf) {
          getline_bufsiz = 4096;
          getline_buf = malloc(getline_bufsiz);
          if (!getline_buf) {
               perror("malloc");
               exit(1);
          }
     }
     char* p = getline_buf;
     int init = 1;
     while (1) {
          int bytes = getline_bufsiz - (p - getline_buf); /* remaining buffer size */
          if (fgets(p, bytes, fp) == NULL) {
               if (errno == 0) {
                    return init ? NULL : getline_buf;
               }
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
