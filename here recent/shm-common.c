#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm-common.h"

/**
 * Displays the version when user passes -v as an argument in a command line.
 * \param isClient A boolean that defines if version should be displayed for client
    or a server and sets local variable char *label to according text.
 */
void display_version(int isClient) {
  char *label;

  label = isClient ? "shm-client 2.11532" : "shm-server 2.11532";

  printf("%s\n\n", label);
  printf("%s\n\n", "Copyright (C) 2019 Golovan Mykola and Cadeddu Mathieu.");
  printf("%s\n", "Written by Golovan Mykola <mykola.golovan@univ-pau.fr> and Cadeddu Mathieu <mathieu.cadeddu@univ-pau.fr>.");
}

void display_setup(char *pathname, int proj_id) {
  printf("proj_id = \"%d\"\n", proj_id);
  printf("pathname = \"%s\"\n", pathname);
}

key_t generate_key(char *pathname, int proj_id) {
  key_t key = -1;

  key = ftok(pathname, proj_id);

  return(key);
}
