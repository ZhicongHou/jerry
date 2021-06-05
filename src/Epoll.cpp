

#include "Epoll.h"


Epoll::Epoll(){
    this->ep_fd = epoll_create(100);
    if(this->ep_fd<0){
        perror("epoll_create_thread");
        exit(-1);
    }
}
Epoll::~Epoll(){}

void Epoll::add(int fd){
    // 将客户端的文件描述符放到epoll中监听
    epoll_event cli_epev;
    cli_epev.data.fd = fd; // 之前漏了这一个设置可把自己害惨！！！！
    cli_epev.events = EPOLLIN|EPOLLHUP|EPOLLRDHUP;
    epoll_ctl(this->ep_fd, EPOLL_CTL_ADD, fd, &cli_epev);
}

void Epoll::reomve(int fd){
    epoll_ctl(this->ep_fd, EPOLL_CTL_DEL,fd,NULL);
    close(fd);
}

int Epoll::wait(){
    int n = epoll_wait(this->ep_fd, this->epevs, 1024, -1); //-1代表阻塞的wait
    if(n==-1){
        perror("epoll_wait_thread");
        exit(-1);
    }
    return n; 
}




