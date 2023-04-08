CC = gcc
CFLAGS = $(shell pkg-config --cflags gtk4)
LDFLAGS = $(shell pkg-config --libs gtk4)
TARGET = search-bar
SOURCE = search-bar.c

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
