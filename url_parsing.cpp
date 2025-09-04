#include "http_client.h"
#include "url_parsing.h"
/*
ho capito che strtok_r accetta come parametro str una char str[] ovvero un array statico non un puntatore 
delim sono quei caratteri secondo cui verrà tokenizzata la stringa per esempio : https://www.tutorialspoint.com/articles/index.php" 
delim = "./" ; BISOGNA CONTINUARE LA SPIEGAZIONE 
*/


void print_Host_tokens(struct HOST HOST_LEVEL){
    cout << "----------PRINT HOST LEVEL COMPONENTS----------" << endl;
    for (int i = 0; i < HOST_LEVEL.HOST_TOKENS_LENGTH; i++){
        cout << "HOST LEVEL TOKEN: " << HOST_LEVEL.HOST_TOKENS[i] << endl;
    }
    cout << "HOST_NAME: " << HOST_LEVEL.HOST_NAME << endl;
    cout << "DOMAIN NAME: " << HOST_LEVEL.DOMAIN_NAME << endl;
    cout << "SUBDOMAINN: " << HOST_LEVEL.SUBDOMAIN << endl;
    cout << "TLD: " << HOST_LEVEL.TLD << endl;
    cout << "---------- END PRINTING HOST LEVEL COMPONENTS----------" << endl;
}

void tokenizeTheHost(struct HOST& HOST_LEVEL){
    char delim[] = ".";
    char *token, *saveptr;
    char *HOST_NAME_COPY = new char[1024];
    strcpy(HOST_NAME_COPY, HOST_LEVEL.HOST_NAME);

    token = strtok(HOST_NAME_COPY, delim);
    HOST_LEVEL.HOST_TOKENS_LENGTH = 0;
    while (token){
        HOST_LEVEL.HOST_TOKENS[HOST_LEVEL.HOST_TOKENS_LENGTH] = token;
        token = strtok(NULL, delim);
        HOST_LEVEL.HOST_TOKENS_LENGTH++;
    }
    // VUOL DIRE CHE HO 3 TOKEN SUBDOMAIN , DOMAIN_NAME , TLD
    if(HOST_LEVEL.HOST_TOKENS_LENGTH == 3){
        HOST_LEVEL.SUBDOMAIN = HOST_LEVEL.HOST_TOKENS[0];
        HOST_LEVEL.DOMAIN_NAME = HOST_LEVEL.HOST_TOKENS[1];
        HOST_LEVEL.TLD = HOST_LEVEL.HOST_TOKENS[2];
    }
    // SE ho tre token wuol dire che qualcosa manca un tipo SUBDOMAIN , DOMAIN_NAME , TLD manca , se manca il subDomain va bene il resto non va bene 
    else if(HOST_LEVEL.HOST_TOKENS_LENGTH == 2){
        HOST_LEVEL.DOMAIN_NAME = HOST_LEVEL.HOST_TOKENS[0];
        HOST_LEVEL.TLD = HOST_LEVEL.HOST_TOKENS[1];
    }
    else if(HOST_LEVEL.HOST_TOKENS_LENGTH < 2 ){
        cerr << "INVAILD HOST NAME" << endl;
        exit(1);
    }

    delete[] HOST_NAME_COPY;
    delete token;
    delete saveptr;

    token = NULL;
    saveptr = NULL;
    HOST_NAME_COPY = NULL;
}

void tokenizeTheUrl(struct URL& url_tokens ,char * url ){
    char *token, *saveptr;
    char delim[] = ":/";

    token = strtok_r(url, delim, &saveptr);

    for (int i = 0; token != NULL; i++){
        url_tokens.URL_TOKENS[i] = token;
        url_tokens.URL_TOKENS_LENGTH++;
        token = strtok_r(NULL, delim, &saveptr);
    }

    delete token;
    delete saveptr;
    token = NULL;
    saveptr = NULL;
    // in caso avessi solo il DOMAIN NAME 
    /*Però va controllato che il DOMAIN NAME sia impostato bene */
    cout << url_tokens.URL_TOKENS_LENGTH << endl;
    

    if (url_tokens.URL_TOKENS_LENGTH == 1)
    {
        url_tokens.HOST_LEVEL.HOST_NAME = url_tokens.URL_TOKENS[0];
        // Faccio il parsing del Domain Name , ho già una struct HOST OVVERO HOST_LEVEL
        tokenizeTheHost(url_tokens.HOST_LEVEL);
    }

    url_tokens.HOST_LEVEL.HOST_NAME = url_tokens.URL_TOKENS[1];

}