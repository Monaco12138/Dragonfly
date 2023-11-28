#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h> // struct sockaddr_in
#include <unistd.h>
#include <arpa/inet.h> // inet_ntop
#include <cstring>
#define REQUEST_MAX_LENGTH 6

int main() {
    std::cout << "Servering..." << std::endl;

    //socket init
    uint8_t socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if ( socketFileDescriptor <= 0 ){
        std::cout << "socket failed" << std::endl;
        return 0;
    }

    int opt = 1;
    if ( setsockopt(socketFileDescriptor, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        std::cout << "socket reuse addr failed" << std::endl;
        return 0;
    }
    if ( setsockopt(socketFileDescriptor, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cout << "socket reuse port failed" << std::endl;
        return 0;
    }

    // ipv4 struct
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons( 7777 );

    // bind
    if ( bind(socketFileDescriptor, (struct sockaddr*)& addr, sizeof(addr)) < 0 ) {
        std::cout << "socket bind failed" << std::endl;
        return 0;
    }

    // listen
    if ( listen(socketFileDescriptor, 3) < 0 ) {
        std::cout << "Error: listen" << std::endl;
        return 0;
    }

    // accept
    uint8_t connSocket;
    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof( clientAddr );
    char clientIP[20] = "";
    
    while( true ) {
        std::cout << "...listening" << std::endl;
        connSocket = accept( socketFileDescriptor, (struct sockaddr*)& clientAddr, (socklen_t*)& clientAddrLen );
        if ( connSocket < 0 ) {
            std::cout << "Error: accept" << std::endl;
            continue;
        }

        // 网络字节struct 转换为 string
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        std::cout << "...connect " << clientIP << ":" << ntohs(clientAddr.sin_port) << std::endl;

        char buf[REQUEST_MAX_LENGTH];
        while( true ) {
            memset(buf, 0, sizeof(buf));
            int len = recv(connSocket, buf, sizeof(buf), 0);
            buf[len] = '\0';
            if (strcmp(buf, "exit") == 0) {
                std::cout << "...disconnect " << clientIP << ":" << ntohs(clientAddr.sin_port) << std::endl;
                break;
            }
            std::cout << "Receive from client: ";
            std::cout << buf << std::endl;
            send(connSocket, buf, len, 0);
        }
        close( connSocket );
    }   

    close( socketFileDescriptor );

    return 0;
}