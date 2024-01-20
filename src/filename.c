#include "filename.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

char* filename = NULL;
char* default_basename = ".cdhist";

char* cdhist_filename() {
     if (filename) {
          return filename;
     }
     if ((filename = getenv("CDHIST_FILE"))) {
          if (!(filename = strdup(filename))) {
               perror("strdup");
               exit(1);
          }
          return filename;
     }
     char* home = getenv("HOME");
     if (home) {
          if (!(filename = malloc(strlen(home) + 1 +
                                  strlen(default_basename) + 1))) {
               perror("malloc");
               exit(1);
          }
          strcpy(filename, home);
          strcat(filename, "/");
          strcat(filename, default_basename);
          return filename;
     }
     fprintf(stderr, "no home directory\n");
     exit(1);
}
