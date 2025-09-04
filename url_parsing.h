#ifndef URL_PARSING_H
#define URL_PARSING_H

#define BUFF_SIZE 1024
#define MAX_BUFF_SIZE 4096

struct HOST{
    char *HOST_NAME;
    char *SUBDOMAIN;
    char *DOMAIN_NAME;
    char *TLD;
    char **HOST_TOKENS = new char*[BUFF_SIZE];
    int HOST_TOKENS_LENGTH;
};

struct URL{
    

    char *PROTOCOL;
    struct HOST HOST_LEVEL;
    char **SUB_DIRECTORIES = new char*[BUFF_SIZE];
    char **URL_TOKENS = new char*[BUFF_SIZE];
    int URL_TOKENS_LENGTH = 0;
};

void tokenizeTheUrl(struct URL&  url_tokens ,char* url );
void print_Host_tokens(struct HOST  HOST_LEVEL);
void tokenizeTheHost(struct HOST &HOST_LEVEL);
#endif