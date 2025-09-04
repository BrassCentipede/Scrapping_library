
/* Lavoriamo a come inviare una richiesta HTTP a un server e ricevere una risposta usando i socket e le sytem call

Senza librerie esterne per ora niente libcurl per ora vorrei capire come funziona da zero per imparare per davvero qualcosa di utile 
Vorrei capire un po meglio come funziona la programmazione a basso livello 
*/
#include "http_client.h"
#include "url_parsing.h"
using namespace std;

int OPEN_SSL_free_all(SSL *ssl , BIO *b , SSL_CTX * ctx){
    SSL_shutdown(ssl);
    BIO_free_all(b);
    SSL_CTX_free(ctx);
    return 0;
}
void report_and_exit(const char* msg){
    //last error encountered during a call to a system or library function
    //perror() = voce del sistema operativo che ti dice “non sono riuscito a fare la syscall X, motivo Y”.
    perror(msg);
    /*OpenSSL è complicata: ogni volta che qualcosa va storto (handshake fallito, certificato non valido, cifrario non supportato, ecc.), la libreria non ti dà solo un “-1” ma mette i dettagli in una coda interna di errori.
    Tu puoi:
    Leggere questa coda con funzioni tipo ERR_get_error().
    Oppure, più semplicemente, farla stampare tutta a video con ERR_print_errors_fp().*/
    ERR_print_errors_fp(stderr);// mi da gli errori legati a ssl sullo standard output
    exit(-1);
}
void HTTPS_connection_1(const char * hostname){
    //Creazione dei Buffer per i dati 
    char name[BUFF_SIZE];
    char request[BUFF_SIZE];
    char response[BUFF_SIZE];
    //Scelgo la versione di TLS , SE SONO CLIENT O SERVER E COSI VIA 
    const SSL_METHOD *method = TLS_client_method(); // sceglie lui la versione migliore/ultima 
    if(method == NULL){report_and_exit("TLS_client_method...");}
    // Crei il contesto per poter creare delle connessioni cifrate 
    SSL_CTX *ctx = SSL_CTX_new(method);
    if(ctx == NULL){report_and_exit("SSL_CTX_new...");}

    #ifdef __Hello__
    if(!SSL_CTX_set_cipher_list(ctx , "DEFAUL:@SECLEVEL=1")){
        report_and_exit("SSL_CTX_set_cipher_list... ");
    }
    #endif

    // Creo un bio e lui si occuperà di creare un socket TCP e fare un po' di lavoro dietro le quinte 
    BIO *bio =BIO_new_ssl_connect(ctx);
    if(bio == NULL){report_and_exit("BIO_new_ssl_connect...");}

    SSL *ssl = NULL;
    sprintf(name, "%s:%s", hostname, "https");
    cout << name << endl;
    BIO_get_ssl(bio, &ssl);                // questo ssl punterà al ssl che c'è dentro bio , così posso modificare impostazioni specifice all'interno del bio tramite ssl creato da me
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);// per la robustezza in caso di errore 
    BIO_set_conn_hostname(bio, name); // prova una connessione

    if(!SSL_set_tlsext_host_name(ssl , hostname)){
        report_and_exit("SSL_set_tlsext_hostname...");
    }

    if(BIO_do_connect(bio) <= 0){ // corrisponde a connect() dei socket 
        report_and_exit("BIO_do_connect...");
    }

    if(!SSL_CTX_load_verify_locations(ctx , CERT_LOCATION , NULL)){ // qua trova i certificati di fiducia CA
        report_and_exit("SSL_CTX_load_verify_location()");
    }

    long verify_flag = SSL_get_verify_result(ssl); // verifica che il certificato sia valido 
    if (verify_flag != X509_V_OK){
        fprintf(stderr, "Certificate verification error (%i) but continuing...\n", (int)verify_flag);
    }

    // ora ci occupiamo dei dati in se

    sprintf(request, "GET / HTTP/1.1\x0D\x0AHost: %s\x0D\x0A\x43onnection: Close\x0D\x0A\x0D\x0A",hostname);
    BIO_puts(bio, request);
    /*
    cout << "ONE TIME RESPOONCE" << endl;
    int n = BIO_read(bio, response, BUFF_SIZE); // prende 1024 byte di contenuto
    cout << response << endl;*/
    while(1){
        memset(response, '\0', sizeof(response)); // azzera/svuota il buffer 
        int n = BIO_read(bio, response, BUFF_SIZE); // prende 1024 byte di contenuto 
        if (n <= 0) break; // se i byte presi sono <= 0 mi fermo
        cout << n << endl;
        puts(response); // se non entro in quel if con il break allora stampo 
    }

    if(OPEN_SSL_free_all(ssl ,bio , ctx ) != 0){
        perror("SOMETHING WENT WRONG WHILE FREEING MEMORY");
    }
}

void HTTPS_connection_2(struct URL URL_TOKENS){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // creazione del socket
    if(sockfd < 0){ // controllo che il socket è stato creato come si deve 
        cerr << "Error creating socket" << endl;
        exit(1); // ovvero l'errore del fail
    }
    // Creo la criptografia , ovvero scelgo il tipo di TlS la sua versione , viene fatto automaticamente 
    const SSL_METHOD * method = TLS_client_method();
    if(!method){report_and_exit("TLS_client_method() ...");}

    SSL_CTX *ctx = SSL_CTX_new(method);
    if(!ctx){report_and_exit("SSL_CTX_new() ...");}

    SSL *ssl = SSL_new(ctx);
    if(!ssl){report_and_exit("SSL_new()...");}

    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);// per la robustezza in caso di errore 
    
    // Server address structure
    struct sockaddr_in serv_addr;
    // serve per svuotare i campi della struct cosi non ho roba inutile al suo interno
    // svuota i campi molto easy 
    memset(&serv_addr, 0, sizeof(serv_addr)); // svuota del tutto grazie a sizeof(...)
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(HTTPS_PORT); // trasforma in big endian come vuole la rete , host to network 

    // Resolve server address
    struct hostent *server_info = gethostbyname(URL_TOKENS.HOST_LEVEL.HOST_NAME);
    if(server_info == nullptr){
        cerr << "No such host " << endl;
        exit(1);
    }

    memcpy(&serv_addr.sin_addr.s_addr, server_info->h_addr, server_info->h_length); // copio delle informazioni nella struct 

    struct in_addr **addr_list;
    addr_list = (struct in_addr **)server_info->h_addr_list;

    for (int i = 0; addr_list[i] != NULL; i++) {
        cout << (&addr_list[i]) << endl;
        cout << i + 1 << " " << inet_ntoa(*addr_list[i]) << endl;
    }

    if(connect(sockfd , (struct sockaddr*)&serv_addr , sizeof(serv_addr))  < 0){
        cerr << "Error connecting to the server " << endl;
        exit(1);
    };

    SSL_set_fd(ssl, sockfd);
    SSL_connect(ssl);

    if(!SSL_CTX_load_verify_locations(ctx , CERT_LOCATION , NULL)){ // qua trova i certificati di fiducia CA
        report_and_exit("SSL_CTX_load_verify_location()");
    }

    long verify_flag = SSL_get_verify_result(ssl); // verifica che il certificato sia valido 
    if (verify_flag != X509_V_OK){
        fprintf(stderr, "Certificate verification error (%i) but continuing...\n", (int)verify_flag);
    }

    const char *request = "GET / HTTP/1.1\r\nHost: ";
    const char *conection_close = "\r\nConnection: close\r\n\r\n";
    char result_request[BUFF_SIZE];
    strcpy(result_request, request);
    strcat(result_request, URL_TOKENS.HOST_LEVEL.HOST_NAME);
    strcat(result_request, conection_close);

    if(SSL_write(ssl , result_request , strlen(result_request)) < 0){
        cerr << "error handling request" << endl;
        exit(1);
    }
    // Resive server response
    char buffer[BUFF_SIZE];
    int bytes_received = 0;
    // Anche qua dovrei usarlo 

    while(1){
        memset(buffer, '\0', sizeof(buffer)); // svuoto il buffer 
        int length = SSL_read(ssl, buffer, BUFF_SIZE - 1); // chiamo la funzione e vedo la lunghezza del contenuto 
        if(length <= 0){break;} // se il contenuto ricevuto <= 0 vuol dire che i dati ricevuti sono finiti , quindi mi fermo , senza questo controllo andrebbe all'infinito
        puts(buffer);// stampo + \n con puts
    }
    if (bytes_received < 0) {
        std::cerr << "Error receiving response" << std::endl;
        exit(1);
    }
    close(sockfd);
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);

    cout << "Fine" << endl;
}

int main(int argc , char** argv){

    char url[] = "https://app.joinroster.co"; 
    // per fare tutto questo bisogna fare il parsing dell'url per mandare i dati corretti al socket per la richiesta 
    fprintf(stderr, "Trying an HTTPS connection to %s\n", url); 

    struct URL URL_TOKENS;
    tokenizeTheUrl(URL_TOKENS, url);
    cout << "HOST NAME : "<< URL_TOKENS.HOST_LEVEL.HOST_NAME << endl;

    HTTPS_connection_2(URL_TOKENS);
    // Facciamo il parsing del url

    // char hostname[] va bene anche come argomento per le funzioni con const char* hostname e per le funzioni che hanno solo char * hostname
    // DEVO CAPIRE COME COME FARE IL PARSING DELLE SUB_DIRECTORIES
}