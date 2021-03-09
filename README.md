# CS208-project
Compile server.c and client.c in different machine:
```
$ gcc -o server server.c
$ gcc -o client client.c
```

Run server first:
```
$ ./server
```

Run client with server machine IP:
```
$ ./client 10.10.10.10
```
