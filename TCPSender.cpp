#include "TCPSender.h"


TCPSender::TCPSender() {
    // Initialization
    strcpy(IP_ADDRESS, "127.0.0.1"); // default destination IP address
    PORT = 8081; // default destination port
    // sender's socket
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return;
    }
}

TCPSender::~TCPSender() {
    stopTCP();
    
    if(sock >= 0) { shutdown(sock, SHUT_RDWR); }
}

/**************************************************
 ** Create the thread that connects to TCP server
 **************************************************/
void TCPSender::startTCP() {
    tcpSenderThread = std::thread(&TCPSender::connectTCP, this);
}

/**************************************************
 ** Terminate the thread that connects to TCP server
 **************************************************/
void TCPSender::stopTCP() {
    running = false;
    while(!tcpSenderThread.joinable()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    tcpSenderThread.join();
}

/**************************************************
 ** Assign the destination IP Address and Port
 **************************************************/
void TCPSender::setIP(char *ip, int port) {
    memcpy(IP_ADDRESS, ip, sizeof(char) * strlen(ip));
    PORT = port;
}


/**************************************************
 ** Connect to the TCP server
 **************************************************/
void *TCPSender::connectTCP() {
    if(sock >= 0) {

        /* =======================================================
         * TODO: Connect to the TCP server
         * ======================================================= */
        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr)); 
        serv_addr.sin_family = AF_INET;  
        serv_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
        serv_addr.sin_port = htons(PORT); 
        connect(sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
        /* ======================================================= */
        
        // Kepp the threat running so we can send data whenever we need
        running = true;
        while(running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    } else {
        printf("There was previously an error in creating a socket or the IP address was unsupported.");
    }
    return 0;
}

/**************************************************
 ** Send data through the TCP connection
 **************************************************/
void TCPSender::sendInteger(int data) {
    if(!running) { return; }

    memcpy(buffer, &data, sizeof(int));
    printf("Send Integer: %d (%lubytes)\n", data, sizeof(int));

    send(sock, buffer, sizeof(int), 0);
}

