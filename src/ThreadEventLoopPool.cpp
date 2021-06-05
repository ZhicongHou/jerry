

#include "ThreadEventLoopPool.h"
#include "Locker.h"
#include "ThreadEventLoop.h"
#include "Reactor.h"
#include "Epoll.h"
#include "Channel.h"
#include "Connection.h"
#include "EventLoop.h"



ThreadPool::ThreadPool(int _max_thread_num, int _max_working_num, int _max_waiting_num, int _max_connection_per_thread):
max_thread_num(_max_thread_num),max_working_num(_max_working_num),
max_waiting_num(_max_waiting_num), max_connection_per_thread(_max_connection_per_thread) {
    locker = new Locker();
    thread_list.clear();
    for(int i = 0; i<max_thread_num; i++){
        /**
         * @brief 踩坑处
         * 这里一开始用了变量thread而不是指针pthread
         * 变量在初试化create时，创建线程传一个this指针，指向当前对象
         * 然后下面的thread_list.push_back中，push_back是值传递（深复制），是全新的一个对象
         * 所以push进thread_list里面的对象不是thread，thread变量在花括号结束时就销毁
         * 造成线程参数this指向的是一个销毁的对象
         */
        Thread * pthread = new Thread(max_connection_per_thread,this);
        pthread->start();
        thread_list.push_back(pthread);
    }
}


ThreadPool::~ThreadPool(){
    for(auto pthread:thread_list){
        delete pthread;
    }
    delete locker;
}

void ThreadPool::init(){
    cur_idx = 0;  
    // 开启channel push
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, ThreadPool::maintain, this);
    if(ret!=0){
        printf("创建maintain线程失败！\n");
        exit(-1);
    }else{
        // 分离线程
        pthread_detach(tid);
        printf("创建maintain线程成功！tid：%lu\n",tid);
    }
}

// 放到请求队列中，要加锁。
void ThreadPool::add_connection(Connection *connection){
    locker->lock();
    if(connection_que.size()>=max_waiting_num){
        close(connection->fd);
        delete connection;
        printf("等待队列已满！拒绝该连接！\n");
    }else{
        connection_que.push(connection);
    }
    locker->unlock();
}

// 删除Connection
void ThreadPool::remove_connection(int fd){
    locker->lock();
    delete connection_map[fd]; //删除connection空间
    connection_map.erase(fd);
    std::cout<<"threadpool delete connection fd:"<<fd<<std::endl;
    locker->unlock();

    
}


// 轮询选择工作线程
void* ThreadPool::maintain(void *args){
    ThreadPool *threadpool = (ThreadPool*)args;
    while(true){
        threadpool->locker->lock();
        // 等待队列不空，且工作connection数没有达到上限
        if(!threadpool->connection_que.empty() && threadpool->connection_map.size()<threadpool->max_working_num){
            Connection *connection = threadpool->connection_que.front();
            threadpool->connection_que.pop();
            // 被选进工作的connection放到map中
            threadpool->connection_map[connection->fd] = connection;  
            //早点释放锁
            threadpool->locker->unlock(); 
            threadpool->thread_list[threadpool->cur_idx]->add_connection(connection);
            threadpool->cur_idx = (threadpool->cur_idx+1)%threadpool->max_thread_num;
        }else{
            threadpool->locker->unlock(); 
        }
        
    } 
}

/**
 * @brief 加一个广播函数。循环每个working_connection，
 * // connection_map[fd]->channel->handle_write(bla bla);
 */