// filename : client.c
// author : Robin Cochennec
// creation : 2018-22-12
// last update : 2018-22-12
// original at : https://gist.github.com/suyash/2488ff6996c98a8ee3a84fe3198a6f85

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERV_PORT 2999

int main(int argc, char** argv) {

    if(argc !=3)
    {
        printf("wrong usage of client, expected : ./client <url> <sequence>");
        return -1;
    }

    const char* server_name = "localhost";
    const int server_port = SERV_PORT;

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;

    // creates binary representation of server name
    // and stores it as sin_addr
    // http://beej.us/guide/bgnet/output/html/multipage/inet_ntopman.html
    inet_pton(AF_INET, server_name, &server_address.sin_addr);

    // htons: port in network order format
    server_address.sin_port = htons(server_port);

    // open a stream socket
    int sock;
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        printf("could not create socket\n");
        return 1;
    }

    // TCP is connection oriented, a reliable connection
    // **must** be established before any data is exchanged
    if (connect(sock, (struct sockaddr*)&server_address,
                sizeof(server_address)) < 0) {
        printf("could not connect to server\n");
        return 1;
    }

    // send
    char* data_to_send = strcpy(data_to_send, argv[1]);
    strcat(strcat(data_to_send, " "), argv[2]);
    printf("Data to send : \'%s\'\n", data_to_send);
    // data that will be sent to the server
    send(sock,(const char*) data_to_send, strlen(data_to_send), 0);

    while(1)
    {

        // receive
        int n = 0;
        int len = 0, maxlen = 100;
        char buffer[maxlen];
        char* pbuffer = buffer;
        char* end_token = "END CONNECTION";
        int over = 0;

        // will remain open until the server terminates the connection
        while ((n = recv(sock, pbuffer, maxlen, 0)) > 0) {
            pbuffer += n;
            maxlen -= n;
            len += n;
            
            buffer[len] = '\0';
            
            printf("%s", buffer);
        }

        if(over!=0)
            break;
    }
    // close the socket
    close(sock);
    return 0;

}
