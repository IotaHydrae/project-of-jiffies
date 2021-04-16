all:server client_1 client_2
server:server.c
	gcc $^ -o $@
client_1:client_1.c
	gcc $^ -o $@
client_2:client_2.c
	gcc $^ -o $@
clean:
	rm server client_1 client_2 -f
