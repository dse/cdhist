#include "append.h"
#include "getline.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/file.h>
#include <unistd.h>
#include <errno.h>

/**
 * Append this_dirname to the cdhist file.  Any previous occurrences
 * of this_dirname in the cdhist file already are removed.
 */
void cdhist_append(char* filename, char* this_dirname) {

     /* generate lock filename (xxx.lock) */
     char* lock_filename = malloc(6 + strlen(filename));
     if (!lock_filename) {
          perror("malloc");
          exit(1);
     }
     strcpy(lock_filename, filename);
     strcat(lock_filename, ".lock");

     /* TODO: really old Linux kernels don't flock() over nfs;
        you must use fcntl in that case. */

     /* create an advisory lock -- FIXME: in really old Linux kernels
        flock() doesn't work over NFS and such; you must use fcntl(). */
     FILE* lock_fh = fopen(lock_filename, "w");
     if (!lock_fh) {
          perror(lock_filename);
          exit(1);
     }
     int lock_fd = fileno(lock_fh);
     if (flock(lock_fd, LOCK_EX) < 0) {
          perror(lock_filename);
          exit(1);
     }

     FILE* fh = fopen(filename, "a+");
     fseek(fh, 0, SEEK_SET);

     int in_sync = 1;           /* are r/w offsets in sync? */

     /* keep track in case we need to overwrite when this_dirname
        is in the file already. */
     long read_pos = 0;
     long write_pos = 0;

     while (1) {
          /* read next line */
          char* line = cdhist_getline(fh);
          if (line == NULL) {
               if (errno == 0) {
                    break;
               }
               perror("cdhist_getline");
               exit(1);
          }
          read_pos = ftell(fh);

          /* not equal to this_dirname? */
          if (strcmp(line, this_dirname)) {
               if (in_sync) {
                    write_pos = read_pos;
                    continue;
               }
               line[len - 1] = '\n';

               /* "append" the line we just read to the write_pos */
               fseek(fh, write_pos, SEEK_SET);
               fputs(line, fh);
               write_pos = ftell(fh);
               fseek(fh, read_pos, SEEK_SET);
               continue;
          }

          /* at this point we encountered this_dirname in the file;
             do no write. */
          if (in_sync) {
               in_sync = 0;
          }
     }

     /* append this directory */
     fseek(fh, write_pos, SEEK_SET);
     fputs(this_dirname, fh);
     fputs("\n", fh);
     ftruncate(fileno(fh), ftell(fh));
     fclose(fh);

     /* release advisory lock */
     flock(lock_fd, LOCK_UN);
     fclose(lock_fh);

     free(lock_filename);
}
