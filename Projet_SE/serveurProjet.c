#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/un.h>

#define MAX_CLIENTS 10
#define MESSAGE_SIZE 100

typedef struct {
  int socket;
  int client_id;
} struct_client;

int cpt_client = 0;
struct_client clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex;

char messages[MAX_CLIENTS][MESSAGE_SIZE];

void *connection_client(void *arg) {
  struct_client client = *((struct_client *) arg);
  int client_id = client.client_id;
  int client_socket = client.socket;
  char client_message[MESSAGE_SIZE];

  while (1) {
    // recevoir un message
    memset(client_message, 0, sizeof(client_message));
    if (read(client_socket, client_message, sizeof(client_message)) == -1) {
      perror("Erreur de lecture du message du client");
      break;
    }

    // mutex pour "enregistrer" le message recu
    pthread_mutex_lock(&clients_mutex);
    strncpy(messages[client_id], client_message, MESSAGE_SIZE);
    // sert a copier le contenu d'un string dans un autre
    pthread_mutex_unlock(&clients_mutex);

    char message_envoye[250];
    sprintf(message_envoye, "Le client ID = %d a envoyé le message suivant: %s", client_id, client_message);

    // envoie message a tous les clients connectés  (avec mutex)
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < cpt_client; i++) {
      if (i != client_id && clients[i].socket != -1) {
        if (write(clients[i].socket, message_envoye, strlen(message_envoye)) == -1) {
          perror("Erreur d'envoi du message au client");
          break;
        }
      }
    }
    pthread_mutex_unlock(&clients_mutex);
  }

  // fermeture connection
  pthread_mutex_lock(&clients_mutex);
  shutdown(client_socket, SHUT_RDWR);
  close(client_socket);
  clients[client_id].socket = -1;
  pthread_mutex_unlock(&clients_mutex);

  pthread_exit(NULL);
}


int main() {
  int client_socket;
  pthread_t client_threads[MAX_CLIENTS];

  // init
  for (int i = 0; i < MAX_CLIENTS; i++) {
    clients[i].socket = -1;
    clients[i].client_id = i;
  }

  // Initialisation struct addr serveur
  struct sockaddr_un serveur_addr = {0};
  serveur_addr.sun_family = AF_UNIX;
  strcpy(serveur_addr.sun_path, "MySock");

  // Initialisation struct addr client
  struct sockaddr_un client_addr = {0};


  // CREATION SOCKET serveur
  int serveur_socket = socket(AF_UNIX, SOCK_STREAM, 0);
  if (serveur_socket  == -1){
      perror("Error creating socket.\n");
      exit(1);
  }

    // bind
  if (bind(serveur_socket, (struct sockaddr *) &serveur_addr, sizeof(serveur_addr)) == -1) {
    perror("erreur de liaison du socket\n");
    exit(EXIT_FAILURE);
  }

    // listen
    if (listen(serveur_socket, MAX_CLIENTS) == -1) {
      perror("erreur d'écoute du socket\n");
      exit(EXIT_FAILURE);
    }

  printf("Le serveur est en attente de connexions...\n");

  while (1) {
    // accepter une connexion client
    socklen_t client_addr_len = sizeof(client_addr);
    if ((client_socket = accept(serveur_socket, (struct sockaddr *) &client_addr, &client_addr_len)) == -1) {
      perror("erreur d'acceptation de la connexion\n");
      continue; // continuer la boucle while sans finir l'iteration
    }

    // si le nombre maximal de clients est atteint
    if (cpt_client >= MAX_CLIENTS) {
      printf("nombre maximal de clients atteint. Nouvelle connexion refusée.\n");
      close(client_socket);
      continue;
    }

    // ajout du client a la liste
    pthread_mutex_lock(&clients_mutex);
    clients[cpt_client].socket = client_socket;
    cpt_client++;
    pthread_mutex_unlock(&clients_mutex);

    printf("Nouvelle connexion acceptée. Bienvenue a l'ID = %d\n", cpt_client - 1);

    // thread client
    pthread_create(&client_threads[cpt_client - 1], NULL, connection_client, (void *) &clients[cpt_client - 1]);
  }

  // Fermer le socket du serveur
  close(serveur_socket);
  return 0;
}


/* sources d'aide :
  - https://www.irif.fr/~sangnier/enseignement/16-17/Reseaux/reseaux-cours6-p4.pdf
  - https://koor.fr/C/Index.wp
  - https://www.tutorialspoint.com/c_standard_library/c_function_strcspn.htm
  - https://docs.oracle.com/cd/E19620-01/805-4041/sockets-47146/index.html
  - https://broux.developpez.com/articles/c/sockets/
  - https://stackoverflow.com/questions/4217037/catch-ctrl-c-in-c
*/
