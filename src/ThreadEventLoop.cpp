

#include "ThreadEventLoop.h"
#include "ThreadEventLoopPool.h"
#include "Epoll.h"
#include "Channel.h"
#include "Connection.h"
#include "EventLoop.h"

Thread::Thread(int _max_channel_num, ThreadPool *_threadPool):
max_channel_num(_max_channel_num),threadPool(_threadPool){
    eventloop = new EventLoop(this);
}
Thread::~Thread(){
    delete eventloop;
}

void Thread::start(){
    int ret = pthread_create(&this->tid, NULL, EventLoop::loop, eventloop);
    if(ret!=0){
        printf("创建线程失败！\n");
        exit(-1);
    }else{
        // 分离线程
        pthread_detach(this->tid);
        printf("创建线程成功！tid：%lu\n",this->tid);
    }
}


void Thread::add_connection(Connection *connection){
    connection->set_eventLoop(eventloop);
    eventloop->add_channel(connection->fd, connection->channel);
}



