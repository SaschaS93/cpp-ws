#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

class Webserver {
    public:
        void run() {
            printf("Starting webserver...");

            int port = 8080;

            int serverSocketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
            if (serverSocketFileDescriptor < 0) {
                printf("Error opening socket! Ending...");
                return;
            }

            // Create empty server address struct
            struct sockaddr_in serv_addr = {};
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_addr.s_addr = INADDR_ANY;
            serv_addr.sin_port = htons(port); // Convert to network byteorder

            int bindReturn = bind(serverSocketFileDescriptor, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
            if (bindReturn < 0) {
                printf("Error binding port! Ending...");
                return;
            }

            listen(serverSocketFileDescriptor, 5);

            // Create empty client address struct
            struct sockaddr_in cli_addr = {};
            socklen_t clilen = sizeof(cli_addr);

            int clientSocketFileDescriptor = accept(serverSocketFileDescriptor, (struct sockaddr *) &cli_addr, &clilen);
            if (clientSocketFileDescriptor < 0) {
                printf("Error creating client socket!");
            }

            char buffer[512] = {};

            int readStatus = read(clientSocketFileDescriptor, buffer, sizeof(buffer)-1);
            if (readStatus < 0) {
                printf("Error reading!");
            }

            printf("Message: %s", buffer);

            string response =   string("HTTP/1.1 200 OK\r\n") +
                                string("Server: simple cpp ws\r\n") +
                                string("Content-Length: 100\r\n") +
                                string("Connection: close\r\n") +
                                string("Content-Type: text/html\r\n") +
                                string("\r\n") +
                                string("<b>Hi!</b>\r\n\r\n");

            int writeStatus = write(clientSocketFileDescriptor, response.c_str(), 512);
            if (writeStatus < 0) {
                printf("Error writing string!");
            }

            close(clientSocketFileDescriptor);
            close(serverSocketFileDescriptor);
        }
};

int main() {
    Webserver* ws = new Webserver();
    ws->run();
    return 1;
}