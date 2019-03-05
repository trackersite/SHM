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
  int c; /* le valeur du résultat de la fonction getopt_long() */
  char *pathname = NULL; /* le chemin du fichier precisé par utilisateur */
  int proj_id = 1; /* numéro du projet */
  shm_message_t *target_message = NULL; /* message cible */
  shm_message_t *source_message = NULL; /* message source */
  key_t key = -1; /* clé de la mémoire partagé */
  int shmid = -1; /* numéro du segment de mémoire */
  int seconds = 1; /* temps pour attendre avant d'envoyer prochaine message */
  int loop_times = 1; /* nombre de fois à boucler */
  char *message_name = NULL; /* nom de message à envoyer */
  char *message_text = NULL; /* text de message à envoyer */

  /* les options possibles du côté client dans la console */
  static struct option long_options[] = {
       {"help", no_argument, 0,  'h' },
       {"key-proj-id", required_argument, 0,  'i' },
       {"message-name", required_argument, 0,  'n' },
       {"key-pathname", required_argument, 0,  'p' },
       {"seconds", required_argument, 0,  's' },
       {"times", required_argument, 0,  't' },
       {"version", no_argument,       0,  'v' },
       {"message-text", required_argument, 0,  'x' },
       {0,         0,                 0,   0  }
   };

   /*  malloc of an alias shm_message_t */
   if ((target_message = malloc(sizeof(shm_message_t))) == NULL) {
       fprintf(stderr, "Error, failed to allocate memory.\n");
       /* free memory in case of an error */
       free(target_message);
       exit(1);
   }

   /*  malloc of an alias shm_message_t */
   if ((source_message = malloc(sizeof(shm_message_t))) == NULL) {
       fprintf(stderr, "Error, failed to allocate memory.\n");
       /* free memory in case of an error */
       free(source_message);
       exit(1);
   }

   if ((message_name = malloc(sizeof(char) * SHM_MESSAGE_NAME_SIZE)) == NULL) {
       fprintf(stderr, "Error, failed to allocate memory for message name.\n");
       /* free memory in case of an error */
       free(message_name);
       exit(1);
   }

   if ((message_text = malloc(sizeof(char) * SHM_MESSAGE_TEXT_SIZE)) == NULL) {
       fprintf(stderr, "Error, failed to allocate memory for message text.\n");
       /* free memory in case of an error */
       free(message_text);
       exit(1);
   }


  if ((pathname = malloc(sizeof(char) * 60)) == NULL) {
    fprintf(stderr, "Error, failed to allocate memory for pathname.\n");
    /* free memory in case of an error */
    free(pathname);
    exit(1);
   }

   strcpy(message_name, "Default name");
   strcpy(message_text, "This is the default message text");
   strcpy(pathname, "file.ftok");

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
          exit(0);
      case 'i':
          proj_id = (int) strtol(optarg, argv, 10); /* convert with cast long to int and assign the result */
          break;
      case 'n':
          strcpy(message_name, optarg);
          break;
      case 'p':
          strcpy(pathname, optarg);
          break;
      case 's':
          seconds = (int) strtol(optarg, argv, 10); /* convert with cast long to int and assign the result */
          break;
      case 't':
          loop_times = (int) strtol(optarg, argv, 10); /* convert with cast long to int and assign the result */
          break;
      case 'v':
          display_version(1);
          exit(0);
      case 'x':
          strcpy(message_text, optarg);
          break;
      default:
          exit(1);
     }
   }

   display_setup(pathname, proj_id);

   /* generate key and verify if it's valid */
   if ((key = generate_key(pathname, proj_id)) == (key_t) -1) {
     printf("./shm-client.out:%s:%d: Unable to create the System V IPC key from the \"%s\" pathname and the \"%d\" project identifier.\n", __FILE__, __LINE__, pathname, proj_id);
     exit(1);
   }

   /* Get memory segment */
   if ((shmid = shmget(key, sizeof(shm_message_t), 0666)) < 0) {
      printf("./shm-client.out:%s:%d: Unable to get the identifier of the System V shared memory segment from the \"0x%x\" key.\n", __FILE__, __LINE__, key);
      exit(1);
   }

   /* Get data */
   target_message = shmat(shmid, NULL, 0);
   strcpy(source_message->name, message_name);
   strcpy(source_message->text, message_text);

   loop_times = (loop_times < 0) ? -1 : loop_times;
   /* set server to listen */
   while (loop_times) {

     /* Interactive mode */
     if (seconds <= 0) {
       printf("Press the Enter key to continue...");
       getchar();

       if (shm_message_copy(*source_message, target_message)) {
         printf("./shm-client.out:%s:%d: Unable to copy the message.\n", __FILE__, __LINE__);
         exit(1);
       }

       shm_message_print(*target_message);
     } else {
       if (shm_message_copy(*source_message, target_message)) {
         printf("./shm-client.out:%s:%d: Unable to copy the message.\n", __FILE__, __LINE__);
         exit(1);
       }
       shm_message_print(*target_message);

       sleep(seconds);
     }
     loop_times--;
   }

  free(pathname);
  pathname = NULL;

  free(message_name);
  message_name = NULL;

  free(message_text);
  message_text = NULL;

  return 0;
}
