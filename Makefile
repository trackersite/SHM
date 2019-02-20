all : shm-client.out shm-server.out

shm-common.o : shm-common.h shm-common.c
	gcc -std=c89 -pedantic -Wall -Werror -D_GNU_SOURCE -g -c -o shm-common.o shm-common.c

shm-message.o : shm-message.h shm-message.c
	gcc -std=c89 -pedantic -Wall -Werror -D_GNU_SOURCE -g -c -o shm-message.o shm-message.c

shm-client.out : shm-client.c shm-common.o shm-message.o
	gcc -std=c89 -pedantic -Wall -Werror -D_GNU_SOURCE -g -o shm-client.out shm-client.c shm-common.o shm-message.o

shm-server.out : shm-server.c shm-common.o shm-message.o
	gcc -std=c89 -pedantic -Wall -Werror -D_GNU_SOURCE -g -o shm-server.out shm-server.c shm-common.o shm-message.o
