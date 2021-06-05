


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


class EventLoop;
class ThreadPool;
class Connection;



class Thread{

public:
    pthread_t tid;
    EventLoop *eventloop;
    ThreadPool *threadPool;
    const int max_channel_num;

public:
    Thread(int max_channel_num,ThreadPool *threadPool);
    ~Thread();
    void start();
    void add_connection(Connection *connection);
};
