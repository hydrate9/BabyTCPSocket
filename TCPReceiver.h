#ifndef TCPReceiver_h
#define TCPReceiver_h

/* =======================================================
 * TODO: Remove or add the library for your platform 
 *       (Windows/Linux/OSX)
 * ======================================================= */
#include <arpa/inet.h>
#include <chrono>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <thread>
#include <pthread.h>
#include <vector>
/* ======================================================= */

class TCPReceiver {
    public:
        int PORT = 8081;
    
        TCPReceiver();
        ~TCPReceiver();
        void startTCP(void);
        void stopTCP(void);
        void setIP(char *ip);
        void sendData(int id, int itvl);

    private:
        int server_fd;  // server socket
        int opt = 1; 
        int addrlen; 
        struct sockaddr_in serv_addr;
        struct sockaddr_in client_address;

        char buffer[1024] = {0};
        bool running = false;
        std::thread tcpReceiverThread;
        std::vector<std::thread> clientThreads;
        int nClients = 0;
        
        void *waitForClients(void);
        void *clientHandler(int clientSocket, int clientIdx);
};
#endif /* TCPReceiver_h */
