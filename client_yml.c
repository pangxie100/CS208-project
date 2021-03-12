#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MAXLIN 4096
 
int main(int argc,char **argv)
{
    int sockfd,rec_len;
    char sendline[4096];
    char buff[MAXLIN];
    struct sockaddr_in servaddr;
 
    if(argc!=2)
    {
        printf("usage:./client <ipaddress>\n");
        exit(0);
    }
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        printf("create socket error:%s(error:%d)\n", strerror(errno),errno);
        exit(0);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(8000);
    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
    {
        printf("inet_pton error for %s\n", argv[1]);
        exit(0);
    }
    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("connect error:%s(errno:%d)\n", strerror(errno), errno);
        exit(0);
    }
    FILE *fpWrite;
    printf("client receive file:(between ==== and ====)\n");
    printf("====\n");
    bzero(buff, sizeof(buff));

    // get file name
    int len = 0;
    if ((rec_len = recv(sockfd,buff,MAXLIN,0)) > 0){
        printf("%s", buff);
        printf("\n");
        printf("------\n");
        int filename_len = strlen(buff);
        char *clear_str = (char *)malloc(filename_len * sizeof(char));
        memcpy(clear_str, buff, filename_len);
        len = strlen(clear_str);
        printf("len_clear_str : %d\n", len);
        if(send(sockfd, clear_str,len,0)==-1){
            perror("send error");
            close(sockfd);
            exit(0);
        }

        char *path = "./result/";
        int path_len = strlen(path);
        printf("path_len = %d\n", path_len);
        char* filepath =(char*)malloc((strlen(path) + strlen(clear_str)) * sizeof(char));
        int filepath_len = strlen(filepath);
        printf("filepath_len = %d\n", filepath_len);
        strcpy(filepath, path);
        strcat(filepath, clear_str);
        fpWrite = fopen(filepath, "w");
        bzero(buff, sizeof(buff));
        free(filepath);
    }

    // get file content
    while ((rec_len = recv(sockfd,buff,MAXLIN,0)) > 0){
        printf("%s", buff);
        fwrite(buff, sizeof(char), rec_len, fpWrite);
        bzero(buff, sizeof(buff));
    }
    printf("====\n");
    fclose(fpWrite);
    close(sockfd);
    return 0;
}
