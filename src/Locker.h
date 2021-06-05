
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



class Locker {
public:
    Locker();
    ~Locker();
    bool lock();
    bool trylock();
    bool unlock();
    pthread_mutex_t *get();

private:
    pthread_mutex_t m_mutex;
};


