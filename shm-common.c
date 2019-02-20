#include <stdio.h>
#include <stdlib.h>
#include "shm-common.h"

/**
 * Displays the version when user passes -v as an argument in a command line.
 * \param isClient A boolean that defines if version should be displayed for client
    or a server and sets local variable char *label to according text.
 */
void displayVersion(int isClient) {
  char *label;

  label = isClient ? "shm-client 2.11532" : "shm-server 2.11532";

  printf("%s\n\n", label);
  printf("%s\n\n", "Copyright (C) 2019 Golovan Mykola and Cadeddu Mathieu.");
  printf("%s\n", "Written by Golovan Mykola <mykola.golovan@univ-pau.fr> and Cadeddu Mathieu <mathieu.cadeddu@univ-pau.fr>.");
}
