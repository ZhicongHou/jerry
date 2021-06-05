

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
class Epoll;
class Thread;
class Locker;


class EventLoop{

private:
    std::unordered_map<int, Channel*> channel_map;
    Epoll *epoll;
    Thread *thread;
    bool is_stop;
    Locker *locker;

public:
    EventLoop(Thread *thread);
    ~EventLoop();


    // void add_channel(Channel channel);
    void add_channel(int fd, Channel *channel);
    void reomve_channel(int fd);
    static void* loop(void *args);
};


