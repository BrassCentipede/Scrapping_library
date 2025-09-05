#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <cstring>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "openssl/bio.h"/* BasicInput/Output streams */
#include <openssl/err.h> /* errors */
#include <openssl/ssl.h> /* core library */

#define HTTP_PORT 80
#define HTTPS_PORT 443
#define BUFF_SIZE 1024
#define MAX_BUFF_SIZE 4096

using namespace std;

#ifdef __linux__
#define CERT_LOCATION "/etc/ssl/certs/ca-certificates.crt"
#elif __APPLE__ || __MACH__
#define CERT_LOCATION  "/opt/homebrew/etc/ca-certificates/cert.pem"
#elif _WIN32 || _WIN64
#include "winHttpClient.h"
#else
#define CERT_LOCATION "certs/cacert.pem"
#endif

struct HTTP_DATA{
    char host[BUFF_SIZE];
    char request[BUFF_SIZE];
    char response[BUFF_SIZE];
};
#endif

