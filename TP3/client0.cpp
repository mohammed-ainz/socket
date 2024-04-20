#include <cstring> 
#include <iostream> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h> 

int main(){
    //création de socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    //définir le serveur d'adresse 
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    //connexion au serveur
    connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    //envoi du message
    const char* message = "Hello, server!";
    send(clientSocket, message, strlen(message), 0);

    //fermer socket
    close(clientSocket);

    return 0;
}