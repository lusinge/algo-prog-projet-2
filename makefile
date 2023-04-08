CC ?= gcc
PKGCONFIG = $(shell which pkg-config)
CFLAGS = $(shell $(PKGCONFIG) --cflags gtk4)
LIBS = $(shell $(PKGCONFIG) --libs gtk4) -lm -g

SRC = search-bar.c gui.c hash.c

OBJS = $(SRC:.c=.o)

all: search-bar.out

%.o: %.c
	$(CC) -c -o $(@F) $(CFLAGS) $<

search-bar.out: $(OBJS)
	$(CC) -o $(@F) $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS)
	rm -f exampleapp
