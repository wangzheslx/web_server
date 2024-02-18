server:main.o web_server.o http_parser.o
	g++ web_server.o http_parser.o main.o  -o server -g
web_server.o:web_server.cpp
	g++ -c web_server.cpp -o web_server.o
http_parser.o:http_parser.cpp
	g++ -c http_parser.cpp -o http_parser.o
main.o:main.cpp
	g++ -c main.cpp -o main.o
.PHONY:clean
clean:
	rm -rf main.o web_server.o http_parser.o server