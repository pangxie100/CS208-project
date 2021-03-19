# CS208-project

## Simple socket
Compile server.c and client.c in different machine:
```
$ cd simple_socket
$ gcc -o server server.c
$ gcc -o client client.c
```

Run server first:
```
$ cd simple_socket
$ ./server
```

Run client with server machine IP (you need to give a right IP address to client.c):
```
$ cd simple_socket
$ ./client 10.10.10.10
```

## Socket for yml
Run server first:
```
$ cd socket_yml
$ ./run_server.sh
```

Run client second (you need to give a right IP address to run_client.sh):
```
$ cd socket_yml
$ ./run_client.sh
```
It will build a connection to the server once. The default file in server is "timestamp.txt" which has three files. Thus, you can run client three times before the server close.

If you want to test multiple times. You can change the file from "timestamp.txt" to "timestamps.txt". (comment the "timestamp.txt" line and uncomment "timestamps.txt" line. Then, comment "char *path = "./";" and uncomment the next line: "char *path = "./keypoints/";") 
Next, server runs first:
```
$ cd socket_yml
$ unzip keypoints.zip
$ ./run_server.sh
```

Client runs this (you need to give a right IP address to test_client_func.c):
```
$ cd socket_yml
$ ./run_client_test.sh
```

## ORB_SLAM3_part
Here are two files which can use socket to send yml file and store them locally when ORB_SLAM3 is running.

## boost_test_cases
These file are used to test the function of boost, like serialization.
Details which are used to install and compile them are shown in the website: https://www.boost.org/doc/libs/1_57_0/more/getting_started/unix-variants.html
