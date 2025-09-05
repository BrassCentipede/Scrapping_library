CC = g++
FLAGS = -Wall 
CPP_VERSION = -std=c++11
HEADER_DIR = ./header
OBJECT_DIR = ./object/
HTTP_CLIENT_DIR = ./httpclient/
URL_PARSING_DIR = ./urlparsing/
OPENSSL_FLAGS = -I./openssl/include openssl/libcrypto.a openssl/libssl.a 



main:
	g++ http_client.cpp url_parsing.cpp -std=c++11 -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto -o http_client

main_2:#compilazione con le librerie statiche  
	g++ http_client.cpp url_parsing.cpp -std=c++11 -I./openssl/include openssl/libcrypto.a openssl/libssl.a  -o http_client

urlParsing_main:
	g++ url_parsing.cpp test_url_parsing.cpp -std=c++11 -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto -o test_url_parsing

urlParsingTest:  $(HTTP_CLIENT_DIR)http_client.cpp $(URL_PARSING_DIR)main_url_parsing.cpp $(URL_PARSING_DIR)url_parsing.cpp
	$(CC) $(FLAGS) $(CPP_VERSION) $(OPENSSL_FLAGS)  $(HTTP_CLIENT_DIR)http_client.cpp $(URL_PARSING_DIR)main_url_parsing.cpp $(URL_PARSING_DIR)url_parsing.cpp -I $(HEADER_DIR) -o $(OBJECT_DIR)url_parsing