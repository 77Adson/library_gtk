CC=gcc
CFLAGS=`pkg-config --cflags gtk+-3.0`
LDFLAGS=`pkg-config --libs gtk+-3.0`

SRC = src/main.c src/magazyn.c
OBJ = $(SRC:.c=.o)
TARGET = magazyn

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)