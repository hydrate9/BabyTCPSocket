#include "TCPReceiver.h"


TCPReceiver::TCPReceiver() {
    PORT    = 8081;
    addrlen = sizeof(serv_addr);
}

TCPReceiver::~TCPReceiver() {
    running = false;
    stopTCP();
}

/**************************************************
 ** Create the thread that waits for TCP connection
 **************************************************/
void TCPReceiver::startTCP() {
    tcpReceiverThread = std::thread(&TCPReceiver::waitForClients, this);
}


/**************************************************
 ** Terminate the threads that 
 **   a) waits for TCP connection
 **   b) connects to clients
 **************************************************/
void TCPReceiver::stopTCP() {
    // a) waits for TCP connection
    while(!tcpReceiverThread.joinable()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    tcpReceiverThread.join();
    // b) connects to clients
    for(int i = 0; i < nClients; i ++) {
        while(!clientThreads[i].joinable()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        clientThreads[i].join();
    }
    nClients = 0;
}

/**************************************************
 ** Create TCP socket and wait for clients to connect
 **************************************************/
void *TCPReceiver::waitForClients() {
    /* =======================================================
     * TODO: Create socket and listen
     * ======================================================= */
    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  
    serv_addr.sin_port = htons(PORT); 
    bind(server_fd, (struct sockaddr*)&serv_addr, addrlen);
    listen(server_fd,10);
    /* ======================================================= */

    // Wait for connection
    running = true;
    while(running) {
        int sock = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t*)&addrlen);
        if(sock >= 0) {
            nClients ++;
            clientThreads.push_back(std::thread( [=] { clientHandler(sock, nClients); } ));
        }
    }

    return 0;
}

void *TCPReceiver::clientHandler(int clientSocket, int clientIdx) {
    int msgIdx = 0;
    while(running) {
        int valread = recv(clientSocket, buffer, 1024, 0);
        if(clientSocket < 0) { running = false; }

        if(valread > 0) {
            msgIdx ++;
            int data;
            memcpy(&data, buffer, sizeof(int));
            printf("[%d|%d|%d] read %d bytes: [%d]\n", clientIdx, nClients, msgIdx, valread, data);

            /* ==================================
             * TEST
             * ================================== */
            if(clientIdx == 1) {
                if(nClients == 1 && msgIdx == 1) {
                    if(data == 10) { printf("==> TEST1 PASSED\n"); }
                    else           { printf("==> TEST FAILED\n");  }
                }
                else if(nClients == 2 && msgIdx == 2) {
                    if(data == 20) { printf("==> TEST3 PASSED\n"); }
                    else           { printf("==> TEST FAILED\n");  }
                }
                else if(nClients == 2 && msgIdx == 3) {
                    if(data == 30) { printf("==> TEST4 PASSED\n"); }
                    else           { printf("==> TEST FAILED\n");  }
                }
            }
            else if(clientIdx == 2) {
                if(data == 99) { printf("==> TEST2 PASSED\n"); }
                else           { printf("==> TEST FAILED\n");  }
            }
            else { printf("==> TEST FAILED\n");  }
        }
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    shutdown(clientSocket, SHUT_RDWR);

    return 0;
}

