
#include "url_parsing.h"
#include "http_client.h"

int main(){


    char url[] ="www.app.joinroster.co";
    struct URL URL_TOKS;

    tokenizeTheUrl(URL_TOKS, url);
    for (int i = 0; i < URL_TOKS.URL_TOKENS_LENGTH; i++){
        cout << "REAL TOKEN: " << URL_TOKS.URL_TOKENS[i] << endl;
    }

    cout << "HOST NAME : " << URL_TOKS.HOST_LEVEL.HOST_NAME << endl;

}