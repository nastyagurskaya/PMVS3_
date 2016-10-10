client:
	gcc client.c -o client
server-thread:
	gcc -pthread server.c -o server
server-process:
	gcc server.c -DPROCESS -o server
clean:
	rm -f client server
