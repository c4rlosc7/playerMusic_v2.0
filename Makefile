# export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/utp/zmq/lib
#ZMQDIR = /home/utp/zmq
ZMQDIR = /home/local/zmq

CC=clang++ -std=c++11 -Wall 
LDFLAGS= -lpthread -lzmqpp -lzmq -I$(ZMQDIR)/include -L$(ZMQDIR)/lib 

all: client broker server

client: client.cc
	$(CC) -o client client.cc $(LDFLAGS)

broker: broker.cc	
	$(CC) -o broker broker.cc $(LDFLAGS)

server: server.cc
	$(CC) -o server server.cc $(LDFLAGS)	
	
clean:
	rm -f client broker server