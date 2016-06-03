CC= gcc
CFLAGS = -Wall
LD = -lssl -lcrypto
s68407: s68407.c
	gcc $(CFLAGS) -o s68407 s68407.c $(LD)

clean:
	rm -f *.o s68407
