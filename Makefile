CC=gcc
CFLAGS=-g -Wall
CCLIBS=
BINS=spqr

all: $(BINS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $^ $(CCLIBS)

clean:
	rm -rf *.dSYM $(BINS)