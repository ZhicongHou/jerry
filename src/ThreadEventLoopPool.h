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

class ThreadPool;
class Locker;
class Connection;
class Thread;



class ThreadPool{


private:
    int max_thread_num;
    int cur_idx;
    std::vector<Thread*> thread_list;
    Locker *locker;
    std::queue<Connection*> connection_que;
    std::unordered_map<int, Connection*> connection_map;
    int max_waiting_num;
    int max_working_num;
    int max_connection_per_thread;


public:
    ThreadPool(int _max_thread_num, int _max_working_num, int _max_waiting_num, int _max_connection_per_thread);
    ~ThreadPool();
    void init();
    void add_connection(Connection *connection);
    void remove_connection(int fd);
    static void* maintain(void *args);
    
};

