

#include "EventLoop.h"
#include "Epoll.h"
#include "Locker.h"
#include "Channel.h"
#include "ThreadEventLoop.h"
#include "ThreadEventLoopPool.h"

EventLoop::EventLoop(Thread *_thread):thread(_thread){
    epoll = new Epoll();
    locker = new Locker();
}
EventLoop::~EventLoop(){
    delete epoll;
    delete locker;
}


void EventLoop::add_channel(int fd, Channel *channel){
    
    // epoll也要加锁，不然会segmentation fault段错误
    /**
     * @brief 注意：这里的epoll->add放在前面和后面是会有问题的。放在前面，会残生死锁。
     * 在add时，主线程获得epoll的锁，下一步想获得map的locker锁，但locker锁已经被eventloop获取了。
     * 而eventloop想remove，获得了locker，下一步是获取epoll的锁。这样就会造成循环等待。so?????
     * 
     */
    locker->lock();
    epoll->add(fd);
    channel_map[fd] = channel;
    locker->unlock();
    
}


void EventLoop::reomve_channel(int fd){
    // 删除channel的时候，要在threadPool中把对应的Connection也删除
    thread->threadPool->remove_connection(fd);
    locker->lock();
    epoll->reomve(fd);
    channel_map.erase(fd);
    locker->unlock();
}


void* EventLoop::loop(void *args){
    EventLoop* eventLoop = (EventLoop *)args;
    Channel *channel = NULL;
    while(true){
        int n = eventLoop->epoll->wait();
        struct epoll_event *epevs = eventLoop->epoll->epevs;
        for(int i = 0; i<n; i++){
            int fd = epevs[i].data.fd;
            eventLoop->locker->lock();
            channel = eventLoop->channel_map[fd];
            eventLoop->locker->unlock();
            channel->handle(epevs[i].events);
        }
    }
}

