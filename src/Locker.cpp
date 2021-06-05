

#include "Locker.h"

// 线程同步机制封装类
Locker::Locker() {
    if(pthread_mutex_init(&m_mutex, NULL) != 0) {
        throw std::exception();
    }
}

Locker::~Locker() {
    pthread_mutex_destroy(&m_mutex);
}

bool Locker::lock() {
    // trylock为非阻塞的互斥锁类型
    return pthread_mutex_lock(&m_mutex) == 0;
}

bool Locker::trylock() {
    
    return pthread_mutex_trylock(&m_mutex) == 0;
}

bool Locker::unlock() {
    return pthread_mutex_unlock(&m_mutex) == 0;
}

pthread_mutex_t *Locker::get()
{
    return &m_mutex;
}




