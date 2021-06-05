

#include <cstdio>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include <errno.h>
#include <iostream>
#include <stdint.h>
#include <pthread.h>
#include <vector>
#include <queue>
#include <functional>
#include <sys/epoll.h>
#include <map>
#include <unordered_map>
#include <exception>
#include <semaphore.h>


class Channel;
class EventLoop;
class Connection{

private:
    char read_buff[1024];


public:
    int fd;
    char *ip;
    uint16_t port;
    EventLoop *eventLoop;    //这个Connection属于哪个eventloop管辖的
    Channel *channel;

public:
    Connection(int _fd, char *_ip, uint16_t _port);
    ~Connection();
    void set_eventLoop( EventLoop *_eventLoop);
    void handle_close();
    void handle_read();
    void handle_write();
};

