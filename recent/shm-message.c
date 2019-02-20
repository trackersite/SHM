#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "shm-message.h"

int shm_message_copy(shm_message_t message_source, shm_message_t *message_target)
{
  if(shm_message_is_empty(*message_target) == 0)
  {
    return -1;
  }

  if(shm_message_set_name(message_target, message_source.name) == -1)
  {
    return -1;
  }

  if(shm_message_set_text(message_target, message_source.text) == -1)
  {
    return -1;
  }

  return 0;
}

void shm_message_empty(shm_message_t *message)
{
  message->name[0] = '\0';
  message->text[0] = '\0';
}

int shm_message_is_empty(shm_message_t message) {
    return (message.name[0] == '\0' && message.text[0] == '\0');
}

void shm_message_print(shm_message_t message)
{
  struct tm *currentTime = NULL;
  time_t rawtime;

  time(&rawtime);

  currentTime = localtime(&rawtime);

  printf("%d-", currentTime->tm_year+1900);

  if (currentTime->tm_mon+1 < 10) {
    printf("0");
    printf("%d-", currentTime->tm_mon+1);
  } else {
    printf("%d-", currentTime->tm_mon+1);
  }


  if (currentTime->tm_mday < 10){
    printf("0");
    printf("%d ", currentTime->tm_mday);
  } else {
    printf("%d ", currentTime->tm_mday);
  }

  if (currentTime->tm_hour < 10){
    printf("0");
    printf("%d:", currentTime->tm_hour);
  } else {
    printf("%d:", currentTime->tm_hour);
  }

  if (currentTime->tm_min < 10){
    printf("0");
    printf("%d:", currentTime->tm_min);
  } else {
    printf("%d:", currentTime->tm_min);
  }

  if (currentTime->tm_sec < 10){
    printf("0");
    printf("%d ", currentTime->tm_sec);
  } else {
    printf("%d ", currentTime->tm_sec);
  }

  printf("%s:", message.name);
  printf(" %s\n", message.text);
}

int shm_message_set_name(shm_message_t *message, const char *name) {
  if (strlen(name) > 16) {
    printf("This message text is a bit too long to be accepted. That is sad. ");
    printf("Because its %ld size is greater than \"16\"\n.", strlen(name));
    exit(1);
  }

  strcpy((*message).name, name);

  return 0;
}

int shm_message_set_text(shm_message_t *message, const char *text) {
  if (strlen(text) > 64) {
    printf("This message text is a bit too long to be accepted. That is sad.\n");
    printf("Because its %ld size is greater than \"64\"\n.", strlen(text));
    exit(1);
  }

  strcpy((*message).text, text);

  return 0;
}
