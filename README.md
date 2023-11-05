# Messagerie Instantanée

## About
Projet en C en 7 étapes - Permettant de faire communiquer des terminaux entre eux.

## Contenant
Voici un fichier Projet_SE constitué :
- d'un fichier serveurProjet.c
- d'un fichier clientProjet.c

## Installation
1. Telecharger le fichier zip Projet_SE
2. Lancer le projet:

	a) Ouvrer deux terminaux au moins, et pour chacun "dirigez vous" vers le fichier Projet_SE

	Nous vous recommandons de le placer dans vos Documents et d'executer les commandes suivantes:
  		cd Documents
  		cd Projet_SE

  	b) Exécuter et compliler le serveur dans un des terminaux:
      	unlink MySock (si vous avez un fichier de ce nom dans le dossier, en general apres la 1ere utilisation de la messagerie )
      	clang -Wall serveurProjet.c
  		./a.out

  	c) Exécuter et compliler des clients ( max 10 ) d'autres terminaux (un client par terminal):
		clang -Wall serveurProjet.c
  		./a.out


3. La commande ctrl+c permet interruption de l’affichage  Des qu'un message saisi il affiche tous les messages qu’il a reçu durant l’interruption.


## Status du Projet
Terminé, malgré quelques incertitudes sur l'affichage des messages reçus après l'interruption.
Il faut appuyer de nouveau sur ctrl+c pour les voir et le 1er message envoyé apres l'interruption est tout de meme recu

### AUTEURS
Noam TORDJMAN
Carla NACHMAN

###SOURCES
- https://www.irif.fr/~sangnier/enseignement/16-17/Reseaux/reseaux-cours6-p4.pdf
- https://koor.fr/C/Index.wp
- https://www.tutorialspoint.com/c_standard_library/c_function_strcspn.htm
- https://docs.oracle.com/cd/E19620-01/805-4041/sockets-47146/index.html
- https://broux.developpez.com/articles/c/sockets/
- https://stackoverflow.com/questions/4217037/catch-ctrl-c-in-c
