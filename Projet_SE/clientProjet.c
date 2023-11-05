#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/un.h>

#define MESSAGE_SIZE 100

int test_ctrlC = 1;
int count = 0;
char mess_ctrlC[1000];

void interruption_test_ctrlC(int signal){
  if (signal == SIGINT){
    test_ctrlC = !test_ctrlC;
    if (!test_ctrlC){
      printf("interruption d'affichage des messages\n");
    }
    else if (count){
      printf("voici les messages reçus pendant l'interruption :\n%s\n", mess_ctrlC);
        count = 0;
    }
  }
}

void *fonction_client(void *arg){
  int socket = *((int *) arg);
  char serveur_message[MESSAGE_SIZE];

  while (1) {
    if (test_ctrlC == 1){
      memset(serveur_message, 0, sizeof(serveur_message));
      if (read(socket, serveur_message, sizeof(serveur_message)) == -1) {
        perror("Erreur de lecture du message du serveur\n");
        break;
      }
      printf("%s\n", serveur_message);
    }
    else {
      strcpy(mess_ctrlC, serveur_message);
      // ici peut etre gerer qq chose par rapport au format des messages ?
      count = 1;
    }
  }
  pthread_exit(NULL);
}


int main(){
  int client_socket;
  pthread_t thread;

  // initialisation de la structure d'adresse du serveur
  struct sockaddr_un serveur_addr = {0};
  serveur_addr.sun_family = AF_UNIX;
  strcpy(serveur_addr.sun_path, "MySock");

  // creation socket
  if ((client_socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("Erreur de création du socket\n");
    exit(EXIT_FAILURE);
  }

  // connect
  if (connect(client_socket, (struct sockaddr *) &serveur_addr, sizeof(serveur_addr)) == -1) {
    perror("Erreur de connexion au serveur\n");
    exit(EXIT_FAILURE);
  }

  signal(SIGINT, interruption_test_ctrlC); // CTRLC

  pthread_create(&thread, NULL, fonction_client, (void *) &client_socket);

  // attend "Entrée" et envoie les messages
  char client_message[MESSAGE_SIZE];
  while (1) {
    if (count) {
      printf("Messages reçus pendant l'interruption :\n%s\n", mess_ctrlC);
      count = 0;
    }

    printf("Vous pouvez envoyer un message quand vous voulez.\n");
    fgets(client_message, MESSAGE_SIZE, stdin);
    client_message[strcspn(client_message, "\n")] = '\0';

    if (write(client_socket, client_message, strlen(client_message)) == -1) {
      perror("Erreur d'envoi du message au serveur\n");
      break;
    }
  }
  close(client_socket);
  return 0;
}
