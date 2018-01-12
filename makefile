all:
	gcc server1.c -o server1.o
	gcc client1.c -o client1.o
clean:
	rm -f *~cliTcp.o servTcp.o