#include "append.h"
#include "filename.h"
#include "getline.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
     if (argc < 3) {
          fprintf(stderr, "not enough arguments");
          exit(1);
     }
     char* filename = argv[1];
     char* dirname = argv[2];
     cdhist_append(filename, dirname);
}
