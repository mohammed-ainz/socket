#include <arpa/inet.h>
#include <dirent.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

void sendDirectoryList(int socket) {
    FILE* lsOutput = popen("ls -lh data | awk '{print $9 , $5}'", "r");
    if (!lsOutput) {
        std::cerr << "Erreur : Impossible d'exécuter la commande ls" << std::endl;
        return;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), lsOutput)) {
        send(socket, buffer, strlen(buffer), 0);
    }

    pclose(lsOutput);
}

int main() {

 while (true) {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

   
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Erreur de connexion au serveur" << std::endl;
        // Attendre un certain temps avant de réessayer la connexion
        usleep(1000000); // Attendre 1 seconde
        continue; // Passer à la prochaine itération de la boucle
    }

    //std::cout << "Connexion établie avec le serveur" << std::endl;

    // Envoie la liste des fichiers au serveur
    sendDirectoryList(clientSocket);
    
    // Fermer le socket après avoir envoyé la liste des fichiers
    close(clientSocket);
    
    // Pause avant de tenter une nouvelle connexion
    usleep(1000000); // Attendre 1 seconde
}

    return 0;
}
