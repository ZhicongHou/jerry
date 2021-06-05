
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

/**
 * @brief 
 * 先声明类，不然在前面的类看不到在后面的类
 */
class Channel;
class Epoll;
class ThreadPool;



class Reactor{

private:
    int sock_fd;
    struct sockaddr_in addr;
    int ep_fd;
    ThreadPool *thread_pool;
    Epoll *epoll;
    Channel *main_channel;



public:

    Reactor();
    ~Reactor();
    void init ();
    void init_socket();
    void run();
    void handle_accept();

};