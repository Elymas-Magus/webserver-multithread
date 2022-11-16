#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h> //structure for storing address information
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>
  
int main(int argc, char const* argv[])
{
    char strData[255];
    int sockD = socket(AF_INET, SOCK_STREAM, 0);
  
    struct sockaddr_in servAddr;
  
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(8090);
    servAddr.sin_addr.s_addr = INADDR_ANY;
  
    int connectStatus = connect(sockD, (struct sockaddr *) &servAddr, sizeof(servAddr));
  
    if (connectStatus == -1) {
        printf("Error...\n");
    } else {
        recv(sockD, strData, sizeof(strData), 0);
        printf("Message: %s\n", strData);
    }
  
    return 0;
}