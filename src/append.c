#include "append.h"
#include "getline.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/file.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include <unistd.h>
#include <fcntl.h>

/* for generating timestamps */
#define TIMESTAMP_STRFTIME    "%a %Y-%m-%dT%H:%M:%S%z"

/* for scanning timestamps to skip them "Fri 2024-01-01T12:34:56-0500" */
#define TIMESTAMP_SCANF       "%8s %u-%u-%uT%u:%u:%u%1[+-]%u %n"

/* for counting fields read by sscanf */
char junkbuf[128];
int junk;

/* for confirming timestamps were read */
#define TIMESTAMP_SCANF_COUNT 9

/* for generating timestamps */
char timebuf[128];

/**
 * Append this_dirname to the cdhist file.  Any previous occurrences
 * of this_dirname in the cdhist file already are removed.
 */
void cdhist_append(char* filename, char* this_dirname) {

     /* generate lock filename (xxx.lock) */
     char* lock_filename = malloc(6 + strlen(filename));
     if (lock_filename == NULL) {
          perror("malloc");
          exit(1);
     }
     strcpy(lock_filename, filename);
     strcat(lock_filename, ".lock");

     /* TODO: really old Linux kernels don't flock() over nfs;
        you must use fcntl in that case. */

     /* create an advisory lock -- FIXME: in really old Linux kernels
        flock() doesn't work over NFS and such; you must use fcntl(). */

     FILE* lock_fh = fopen(lock_filename, "a+");
     if (!lock_fh) {
          fprintf(stderr, "Unable to open lock file\n");
          perror(lock_filename);
          exit(1);
     }
     int lock_fd = fileno(lock_fh);
     if (flock(lock_fd, LOCK_EX) < 0) {
          perror(lock_filename);
          exit(1);
     }

     /**
      * None of the fopen read/write modes fit my requirements.
      * "r+" does not create the file if it does not exist;
      * "w+" truncates the file; and
      * "a+" only appends.
      */

     /* don't truncate the file; create the file if nonexistent */
     FILE* fh = fopen(filename, "a+");
     if (!fh) {
          perror(filename);
          exit(1);
     }
     /* now r+ (which would fail on nonexistent files) will do what I need. */
     fh = freopen(filename, "r+", fh);
     if (!fh) {
          perror(filename);
          exit(1);
     }

     int flags = fcntl(fileno(fh), F_GETFL, NULL);
     if (flags == -1) {
          fprintf(stderr, "cannot get flags\n");
          perror(filename);
          exit(1);
     }
     flags = flags & ~O_APPEND;
     if (-1 == fcntl(fileno(fh), F_SETFL, flags)) {
          fprintf(stderr, "cannot set append flag\n");
          perror(filename);
          exit(1);
     }

     if (fseek(fh, 0, SEEK_SET)) {
          fprintf(stderr, "Unable to seek\n");
          perror(filename);
          exit(1);
     }

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
               perror("error reading\n");
               perror(filename);
               exit(1);
          }
          read_pos = ftell(fh);

          char* dir = line;
          int skip = 0;
          int scanned = sscanf(line, TIMESTAMP_SCANF, junkbuf, &junk, &junk, &junk, &junk, &junk, &junk, junkbuf, &junk, &skip);
          if (scanned == TIMESTAMP_SCANF_COUNT) {
               dir += skip;
          }

          int len = strlen(dir);

          /* (temporarily) remove the terminating newline */
          int term = dir[len - 1];
          if (term == '\n') {
               /* [A] */
               dir[len - 1] = '\0';
          }

          /* not equal to this_dirname? */
          if (strcmp(dir, this_dirname)) {
               if (in_sync) {
                    write_pos = read_pos;
                    continue;
               }
               if (term == '\n') {
                    /* restore string terminator we may have written \0 over [A] */
                    dir[len - 1] = term;
               }

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
     time_t t = time(NULL);
     struct tm* tmp = localtime(&t);
     if (!tmp) {
          perror("error getting time");
          exit(1);
     }
     if (strftime(timebuf, 128, TIMESTAMP_STRFTIME, tmp)) {
          fputs(timebuf, fh);
          fputs(" ", fh);
     }
     fputs(this_dirname, fh);
     fputs("\n", fh);
     ftruncate(fileno(fh), ftell(fh));
     fclose(fh);

     /* release advisory lock */
     flock(lock_fd, LOCK_UN);
     fclose(lock_fh);

     free(lock_filename);
}
