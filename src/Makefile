CC=g++
CFLAGS = -Wall
LIBS = -lzmq -lzmqpp

SERVER_SRC = server.cpp
CLIENT_SRC = client.cpp

SERVER_OBJ = $(SERVER_SRC:.c=.o)
CLIENT_OBJ = $(CLIENT_SRC:.c=.o)

SERVER_EXEC = server
CLIENT_EXEC = client


all: $(SERVER_EXEC) $(CLIENT_EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SERVER_EXEC): $(SERVER_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SERVER_OBJ) $(LIBS)

$(CLIENT_EXEC): $(CLIENT_OBJ)
	$(CC) $(CFLAGS) -o $@ $(CLIENT_OBJ) $(LIBS)

clean:
	rm -rf $(SERVER_EXEC) $(CLIENT_EXEC) *.o *.d
