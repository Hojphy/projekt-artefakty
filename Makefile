CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
OBJ = main.o artifacts.o interface.o
TARGET = archiwum

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
