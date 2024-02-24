CC = gcc

CFLAGS = -Wall

LIBS = -lncurses -lcjson

SRC_CLIENT = socketClient.c
SRC_SERVER = socketServer.c

EXEC_CLIENT = client
EXEC_SERVER = server

all: $(EXEC_CLIENT) $(EXEC_SERVER)

$(EXEC_CLIENT): $(SRC_CLIENT)
	$(CC) $(CFLAGS) $(SRC_CLIENT) -o $(EXEC_CLIENT) $(LIBS)

$(EXEC_SERVER): $(SRC_SERVER)
	$(CC) $(CFLAGS) $(SRC_SERVER) -o $(EXEC_SERVER) $(LIBS)

clean:
	rm -f $(EXEC_CLIENT) $(EXEC_SERVER)