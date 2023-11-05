# Instant Messaging Server

## About
C project in 7 steps - Enabling terminals to communicate with each other.

## Container
Here's a Projet_SE file made up of :
- a file serveurProjet.c
- a  file clientProjet.c

## Installation
Launch the project:

	a) Open at least two terminals, and for each one "point" to the Projet_SE file.
We recommend that you place it in your Documents folder and run the following commands:
  		cd Documents
  		cd Projet_SE

  	b) Run and compile the server in one of the terminals:
      	unlink MySock (if you have a file of this name in the folder, usually after the 1st use of the messaging system)
      	clang -Wall serveurProjet.c
  		./a.out

  	c) Run and compile clients ( max 10 ) from other terminals (one client per terminal):
		clang -Wall serveurProject.c
  		./a.out


3. The ctrl+c command allows you to interrupt the display of any message received during the interruption.


## Project status
Completed, despite some uncertainty about the display of messages received after the interruption.
You have to press ctrl+c again to see them, and the 1st message sent after the interruption is still received.

### AUTHORS
Noam TORDJMAN
Carla NACHMAN

###SOURCES
- https://www.irif.fr/~sangnier/enseignement/16-17/Reseaux/reseaux-cours6-p4.pdf
- https://koor.fr/C/Index.wp
- https://www.tutorialspoint.com/c_standard_library/c_function_strcspn.htm
- https://docs.oracle.com/cd/E19620-01/805-4041/sockets-47146/index.html
- https://broux.developpez.com/articles/c/sockets/
- https://stackoverflow.com/questions/4217037/catch-ctrl-c-in-c
