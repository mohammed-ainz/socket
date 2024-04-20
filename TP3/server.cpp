#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>

#include <fstream> // Pour la manipulation de fichiers

void saveToFile(const char* filename, const char* data, std::size_t size) {
    // Ouvrir le fichier en mode de troncature pour écrire les nouvelles données
    std::ofstream outputFile(filename, std::ios::out | std::ios::binary | std::ios::trunc);
    if (outputFile.is_open()) {
        // Écrire les nouvelles données dans le fichier
        outputFile.write(data, size);
        outputFile.close();
    } else {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << " en mode de troncature" << std::endl;
    }
}

// Fonction de gestion des clients
void* clientHandler(void* clientSocketPtr) {
    int clientSocket = *((int*)clientSocketPtr);

    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);
    getpeername(clientSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);

    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddress.sin_addr, clientIP, INET_ADDRSTRLEN);

    std::cout << "Client connected from IP address: " << clientIP << std::endl;

    // Réception et sauvegarde des fichiers en boucle
    while (true) {
        char buffer[1024];
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            break; // Sortir de la boucle si la connexion est fermée ou une erreur se produit
        }

        // Sauvegarder le fichier dans un fichier log
        saveToFile("server.log", buffer, bytesRead);
    }

    close(clientSocket);
    pthread_exit(NULL);}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    listen(serverSocket, 5);

    while (true) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        pthread_t threadId;
        pthread_create(&threadId, NULL, clientHandler, (void*)&clientSocket);

    }

    close(serverSocket);

    return 0;
}
