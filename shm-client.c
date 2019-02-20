#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include "shm-common.h"
#include "shm-message.h"

int main(int argc, char **argv) {
  int c;
  char *pathname = NULL;
  int proj_id = 1;
  shm_message_t *message = NULL;
  key_t key = -1;
  int shmid = -1;
  int seconds = 1;
  int loopTimes = 1;
  char *messageName = NULL;
  char *messageText = NULL;

  static struct option long_options[] = {
       {"version", no_argument, 0,  'h' },
       {"version", required_argument, 0,  'i' },
       {"version", required_argument, 0,  'n' },
       {"version", required_argument, 0,  'p' },
       {"version", required_argument, 0,  's' },
       {"version", required_argument, 0,  't' },
       {"version", no_argument,       0,  'v' },
       {"version", required_argument, 0,  'x' },
       {0,         0,                 0,   0  }
   };

   /*  malloc of an alias shm_message_t */
   if ((message = malloc(sizeof(shm_message_t))) == NULL) {
       fprintf(stderr, "Error, failed to allocate memory.\n");
       /* free memory in case of an error */
       free(message);
       exit(EXIT_FAILURE);
   }

   if ((messageName = malloc(sizeof(char) * SHM_MESSAGE_NAME_SIZE)) == NULL) {
       fprintf(stderr, "Error, failed to allocate memory for message name.\n");
       /* free memory in case of an error */
       free(messageName);
       exit(EXIT_FAILURE);
   }

   if ((messageText = malloc(sizeof(char) * SHM_MESSAGE_TEXT_SIZE)) == NULL) {
       fprintf(stderr, "Error, failed to allocate memory for message text.\n");
       /* free memory in case of an error */
       free(messageText);
       exit(EXIT_FAILURE);
   }

   messageName = "Default name";
   messageText = "This is the default message text";
   pathname = "file.ftok";

   while ((c = getopt_long(argc, argv, "hi:n:p:s:t:vx:", long_options, NULL)) && (c != -1)) {
     switch (c) {
       case 'h':
          printf("%s\n", "Usage: ./shm-client.out [OPTION]...");
          printf("%s\n\n", "Send a message to a server through shared memory.");

          printf("%s\n\t", "Options:");
          printf("%s\n\t\t", "-h, --help");
          printf("%s\n\t", "display this help and exit");
          printf("%s\n\t\t", "-i, --key-proj-id=PROJ_ID");
          printf("%s\n\t", "set the key project identifier to PROJ_ID (the default value is \"1\")");
          printf("%s\n\t\t", "-n, --message-name=NAME");
          printf("%s\n\t", "set the message name to NAME (the default value is \"Default name\")");
          printf("%s\n\t\t", "-p, --key-pathname=PATHNAME");
          printf("%s\n\t", "set the key pathname to PATHNAME (the default value is \"file.ftok\")");
          printf("%s\n\t\t", "-s, --seconds=SECONDS");
          printf("%s\n\t", "set the seconds between each try (the default value is \"1\", a value less than or equal to 0 enables the interactive mode where the input stream is read)");
          printf("%s\n\t\t", "-t, --times=TIMES");
          printf("%s\n\t", "set the number of times this program tries to send a message (the default value is \"1\", a negative value means repeat for ever)");
          printf("%s\n\t\t", "-v, --version");
          printf("%s\n\t", "output version information and exit");
          printf("%s\n\t\t", "-x, --message-text=TEXT");
          printf("%s\n\n", "set the message text to TEXT (the default value is \"This is the default messagetext\")");
          printf("%s\n", "Report bugs to Golovan Mykola <mykola.golovan@univ-pau.fr> and Cadeddu Mathieu <mathieu.cadeddu@univ-pau.fr>");
          break;
      case 'i':
          proj_id = (int) strtol(optarg, argv, 10); /* convert with cast long to int and assign the result */
          break;
      case 'n':
          messageName = optarg;
          break;
      case 'p':
          pathname = optarg;
          break;
      case 's':
          seconds = (int) strtol(optarg, argv, 10); /* convert with cast long to int and assign the result */
          break;
      case 't':
          loopTimes = (int) strtol(optarg, argv, 10); /* convert with cast long to int and assign the result */
          break;
      case 'v':
          displayVersion(1);
          break;
      case 'x':
          messageText = optarg;
          break;
      default:
          exit(1);
     }
   }

   printf("proj_id = \"%d\"\n", proj_id);
   printf("pathname = \"%s\"\n", pathname);

   /* generate key and verify if it's valid */
   if ((key = ftok(pathname, proj_id)) == (key_t) -1) {
     perror("ftok");
     exit(1);
   }

   /* Get memory segment */
   if ((shmid = shmget(key, sizeof(shm_message_t), 0666)) < 0) {
       perror("shmget");
       exit(1);
   }

   /* Get data */
   message = shmat(shmid, 0, 0);

   /* Put some things into the memory for the other process to read. */
   shm_message_set_name(message, messageName);
   shm_message_set_text(message, messageText);

   /* set server to listen */
   while (loopTimes > 0) {
     sleep(seconds);

     /* if message is not empty then show it */
     if (!shm_message_is_empty(*message)) {
       /* show message */
       shm_message_print(*message);
     }

     loopTimes--;
   }

  return 0;
}
