main:
	g++ http_client.cpp url_parsing.cpp -std=c++11 -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto -o http_client

main_2:#compilazione con le librerie statiche  
	g++ http_client.cpp url_parsing.cpp -std=c++11 -I./openssl/include openssl/libcrypto.a openssl/libssl.a  -o http_client

urlParsing:
	g++ url_parsing.cpp test_url_parsing.cpp -std=c++11 -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto -o test_url_parsing