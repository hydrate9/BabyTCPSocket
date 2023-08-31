#ifndef TCPSender_h
#define TCPSender_h

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
/* ======================================================= */

class TCPSender {
    public:
        int PORT = 8081;
        char IP_ADDRESS[15] = {0};
        bool running = false;
    
        TCPSender();
        ~TCPSender();
        void startTCP(void);
        void stopTCP(void);
        void setIP(char *ip, int port);
        void sendInteger(int data);

    private:
        int sock = -1;
        struct sockaddr_in serv_addr;
        char buffer[1024] = {0};
        std::thread tcpSenderThread;
    
        void *connectTCP(void);
};
#endif /* TCPSender_h */
