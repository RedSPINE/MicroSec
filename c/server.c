// filename : server.c
// author : Robin Cochennec
// creation : 2018-22-12
// last update : 2018-22-12
// original at : https://gist.github.com/suyash/2488ff6996c98a8ee3a84fe3198a6f85

#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * TCP Uses 2 types of sockets, the connection socket and the listen socket.
 * The Goal is to separate the connection phase from the data exchange phase.
 * */

#define SERVER_PORT 2999
#define MAX_CLIENTS 3
#define CRAWLER_PATH "../shell/crawler.sh"
#define TIME_TO_CRAWL 15

int main(int argc, char *argv[]) {

    // socket address used for the server
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;

    // htons: host to network short: transforms a value in host byte
    // ordering format to a short value in network byte ordering format
    server_address.sin_port = htons(SERVER_PORT);

    // htonl: host to network long: same as htons but to long
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    // create a TCP socket, creation returns -1 on failure
    int listen_sock;
    if ((listen_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        printf("could not create listen socket\n");
        return 1;
    }

    // bind it to listen to the incoming connections on the created server
    // address, will return -1 on error
    if ((bind(listen_sock, (struct sockaddr *)&server_address,
                    sizeof(server_address))) < 0) {
        printf("could not bind socket\n");
        return 1;
    }

    int wait_size = MAX_CLIENTS;  // maximum number of waiting clients, after which
    // dropping begins
    if (listen(listen_sock, wait_size) < 0) {
        printf("could not open socket for listening\n");
        return 1;
    }

    // socket address used to store client address
    struct sockaddr_in client_address;
    int client_address_len = 0;

    // run indefinitely
    while (true) {
        // open a new socket to transmit data per connection
        int sock;
        if ((sock = accept(listen_sock, (struct sockaddr *)&client_address,
                        &client_address_len)) < 0) {
            printf("could not open a socket to accept data\n");
            return 1;
        }

        int n = 0;
        int len = 0, maxlen = 100;
        char buffer[maxlen];
        char *pbuffer = buffer;
        char command[maxlen];
        strcat(strcat(command, CRAWLER_PATH), " ");

        printf("client connected with ip address: %s\n",
                inet_ntoa(client_address.sin_addr));

        //read file
        pid_t pid;
        char ch;
        FILE *fp;


        // keep running as long as the client keeps the connection open
        if ((n = recv(sock, pbuffer, maxlen, 0)) > 0) {

            pbuffer += n;
            maxlen -= n;
            len += n;

            const char* crawler_args[3];
            char* buffercpy = strdup(buffer);
            crawler_args[0] = CRAWLER_PATH;
            crawler_args[1] = strsep(&buffercpy, " \t");
            crawler_args[2] = buffercpy;

            printf("Script : %s\nUrl received : %s\nSequence received : %s\n",
                    crawler_args[0], crawler_args[1], crawler_args[2]);
            
            pid = fork();
            char* const args[]= {crawler_args[0], crawler_args[1], crawler_args[2], NULL};

            if (pid == 0)
            {
                printf("Now crawling\n");
                execv(CRAWLER_PATH, args);
            } 
            else
            {
                sleep(TIME_TO_CRAWL);
                kill(pid, SIGKILL);

                fp = fopen("output.txt", "r");
                memset(buffer,0,sizeof(buffer));
                while((ch = fgetc(fp)) != EOF)
                {
                    strcat(buffer, &ch);
                    if(strlen(buffer) > 90)
                    {
                        send(sock, buffer, strlen(buffer), 0);
                        memset(buffer,0,sizeof(buffer));
                    }
                }
                send(sock, buffer, strlen(buffer), 0);
            }
            char* end_token = "END CONNECTION\0";
            send(sock, end_token, strlen(end_token), 0);

        }
        close(sock);
    }

    close(listen_sock);
    return 0;
}

