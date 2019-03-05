#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include "shm-message.h"
#include "shm-common.h"

int main(int argc, char **argv) {
  int c;
  char *pathname;
  int proj_id = 1;
  int loop_times = -1;
  key_t key = -1;
  int shmid = -1;
  shm_message_t *message = NULL;
  int seconds = 1;

  static struct option long_options[] = {
       {"help", no_argument, 0,  'h' },
       {"key-proj-id", required_argument, 0,  'i' },
       {"key-pathname", required_argument, 0,  'p' },
       {"seconds", required_argument, 0,  's' },
       {"times", required_argument, 0,  't' },
       {"version", no_argument,       0,  'v' },
       {0,         0,                 0,   0  }
   };

   /*  malloc of an alias shm_message_t */
   if ((message = malloc(sizeof(shm_message_t))) == NULL) {
      fprintf(stderr, "Error, failed to allocate memory for struct message.\n");
      /* free memory in case of an error */
      free(message);
      exit(1);
   }

   if ((pathname = malloc(sizeof(char) * 60)) == NULL) {
     fprintf(stderr, "Error, failed to allocate memory for pathname.\n");
     /* free memory in case of an error */
     free(pathname);
     exit(1);
   }

   strcpy(pathname, "file.ftok");

   /* manage arguments of the server */
   while ((c = getopt_long(argc, argv, "hi:p:s:t:v", long_options, NULL)) && (c != -1)) {
     switch (c) {
       case 'h':
          printf("%s\n", "Usage: ./shm-server.out [OPTION]...");
          printf("%s\n\n", "Receive messages from clients through shared memory.");

          printf("%s\n\t", "Options:");
          printf("%s\n\t\t", "-h, --help");
          printf("%s\n\t", "display this help and exit");
          printf("%s\n\t\t", "-i, --key-proj-id=PROJ_ID");
          printf("%s\n\t", "set the key project identifier to PROJ_ID (the default value is \"1\")");
          printf("%s\n\t\t", "-p, --key-pathname=PATHNAME");
          printf("%s\n\t", "set the key pathname to PATHNAME (the default value is \"file.ftok\")");
          printf("%s\n\t\t", "-s, --seconds=SECONDS");
          printf("%s\n\t", "set the seconds between each try (the default value is \"1\", a value less than or equal to 0 enables the interactive mode where the input stream is read)");
          printf("%s\n\t\t", "-t, --times=TIMES");
          printf("%s\n\t", "set the number of times this program tries to receive a message (the default value is \"-1\", a negative value means repeat for ever)");
          printf("%s\n\t\t", "-v, --version");
          printf("%s\n\n", "output version information and exit");
          printf("%s\n", "Report bugs to Golovan Mykola <mykola.golovan@univ-pau.fr> and Cadeddu Mathieu <mathieu.cadeddu@univ-pau.fr>");
          exit(0);
      case 't':
          loop_times = (int) strtol(optarg, argv, 10); /* convert with cast long to int and assign the result */
          break;
      case 's':
          seconds = (int) strtol(optarg, argv, 10); /* convert with cast long to int and assign the result */
          break;
      case 'p':
          strcpy(pathname, optarg);
          break;
      case 'i':
          proj_id = (int) strtol(optarg, argv, 10); /* convert with cast long to int and assign the result */
          break;
      case 'v':
          display_version(0);
          exit(0);
      default:
          exit(1);
     }
   }

   display_setup(pathname, proj_id);

  /* generate key and verify if it's valid */
  if ((key = generate_key(pathname, proj_id)) == (key_t) -1) {
    printf("./shm-server.out:%s:%d: Unable to create the System V IPC key from the \"%s\" pathname and the \"%d\" project identifier.\n", __FILE__, __LINE__, pathname, proj_id);
    exit(1);
  }

  /* Create a segment */
  if ((shmid = shmget(key, sizeof(shm_message_t), 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
      printf("./shm-server.out:%s:%d: Unable to get the identifier of the System V shared memory segment from the \"0x%x\" key.\n", __FILE__, __LINE__, key);
      exit(1);
  }

  /* Verify if segment is valid */
  if (shmat(shmid, 0, 0) == (char*) -1) {
   printf("shmat");
   exit(1);
 }

 /* Attach the segment to the data space */
 message = shmat(shmid, NULL, 0);

 /* set server to listen */
 while (loop_times) {

  /* Interactive mode */
  if (seconds <= 0) {
    printf("Press the Enter key to continue...");
    getchar();

    /* if message is not empty then show it */
    if (!shm_message_is_empty(*message)) {
      /* show message */
      shm_message_print(*message);
      shm_message_empty(message);
    }
  } else {
    sleep(seconds);

    /* if message is not empty then show it */
    if (!shm_message_is_empty(*message)) {
      /* show message */
      shm_message_print(*message);
      shm_message_empty(message);
    }
  }
  loop_times--;
}

  /* destroy the shared memory */
  if ((shmctl(shmid, IPC_RMID, NULL)) == -1) {
    printf("%s:%d: Le segment ne peut pas être detruit.\n", __FILE__, __LINE__);
    exit(1);
  }

  free(pathname);
  pathname = NULL;

  return 0;
}
