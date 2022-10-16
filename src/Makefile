CC=g++
CFLAGS = -Wall
LIBS = -lzmq -lzmqpp

BROKER_SRC = broker/broker.cpp broker/TopicManager.cpp
CLIENT_SRC = client/client.cpp client/message.cpp client/operations.cpp

BROKER_OBJ = $(BROKER_SRC:.cpp=.o)
CLIENT_OBJ = $(CLIENT_SRC:.cpp=.o)

BROKER_EXEC = broker_exec
CLIENT_EXEC = client_exec


all: $(BROKER_EXEC) $(CLIENT_EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BROKER_EXEC): $(BROKER_OBJ)
	$(CC) $(CFLAGS) -o $@ $(BROKER_OBJ) $(LIBS)

$(CLIENT_EXEC): $(CLIENT_OBJ)
	$(CC) $(CFLAGS) -o $@ $(CLIENT_OBJ) $(LIBS)

clean:
	rm -rf $(BROKER_EXEC) $(CLIENT_EXEC) client/*.o client/*.d broker/*.o broker/*.d
