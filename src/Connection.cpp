


#include "Connection.h"
#include "Channel.h"
#include "EventLoop.h"

// 疑问：在构造函数里面可以调用其他成员函数？
Connection::Connection(int _fd, char *_ip, uint16_t _port):
fd(_fd),ip(_ip),port(_port){
    // set channel's callback function
    channel = new Channel();
    channel->set_handle_close(std::bind(&Connection::handle_close,this));
    channel->set_handle_read(std::bind(&Connection::handle_read,this));
    channel->set_handle_write(std::bind(&Connection::handle_write,this));
}

Connection::~Connection(){
    delete[] ip;
    delete channel;
}


void Connection::set_eventLoop( EventLoop *_eventLoop){
    eventLoop = _eventLoop;
}

void Connection::handle_close(){
    printf("IP：%s，Port：%u， fd：%d已经关闭连接！\n", ip, port,fd);
    // channel里面包含eventLoop，这种关系不太好。如果这条语句能在eventloop里面做就好了
    eventLoop->reomve_channel(fd);
}

void Connection::handle_read(){
   ssize_t len = read(fd, read_buff, sizeof(read_buff));
    if(len==-1){
        perror("read");
    }else if(len==0){
        printf("IP：%s，Port：%u， fd：%d已经关闭连接！\n", ip, port,fd);
    }else{
        printf("IP:%s, Port:%u，fd:%d：%s\n", ip, port, fd, read_buff);
    }
}


void Connection::handle_write(){

}