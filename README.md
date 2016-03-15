# Linux_C_Socket_Example

## Just yet another school project.

### How to compile

- make sure you are running `Linux Kernel 2.6` or newer.
- make sure you have `build-essentials` installed on your computer.
- cd [path to repo]
g++ -o server server/server.cpp -std=gnu++11
g++ -o client client/client.cpp -std=gnu++11

### How to Execute

```shell
# Start Server
./server/server
# Start Client
./client/client
```

--------------------------------------------------------------------------------

## Example

```
./server/server
Server Ready!
Client ID [4] connected
buffer = [1 2 3 4 5]
read message [1 2 3 4 5]
number[0] = 1    number[1] = 2    number[2] = 3    number[3] = 4    number[4] = 5
sending Reply [MAX(5), MIN(1)]
buffer = [MAX(5), MIN(1)]
Client Disconnected, Shutting Down.
```

```
./client/client
Please input needed messages:
1 2 3 4 5
sending [1 2 3 4 5] ...
Server Reply = [MAX(5), MIN(1)]
closse connection
```
