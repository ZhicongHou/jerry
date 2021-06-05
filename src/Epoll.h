

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

class Epoll{

private:
    int ep_fd;

public:
    struct epoll_event epevs[1024];


public:
    Epoll();
    ~Epoll();

    void add(int fd);
    void reomve(int fd);
    int wait();
};


