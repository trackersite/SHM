#ifndef SHM_COMMON_H
#define SHM_COMMON_H

void display_version();
void display_setup(char *pathname, int proj_id);
key_t generate_key(char *pathname, int proj_id);

#endif
