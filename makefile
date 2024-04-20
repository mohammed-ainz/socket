all: server client

server: server.cpp
	g++ -Wall server.cpp -o server -pthread

client: client.cpp
	g++ -Wall client.cpp -o client

clean:
	rm -f server client
