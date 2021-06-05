

#include "Channel.h"



Channel::Channel(){

}

Channel::~Channel(){
}

void Channel::handle(uint32_t events){
    if(events&EPOLLHUP || events&EPOLLRDHUP){  // 客户端关闭了链接
        handle_close();
    }else if(events&EPOLLIN){  // 客户端有新消息过来
        handle_read();
    }else if(events&EPOLLOUT){
        handle_write();
    }
}

void Channel::set_handle_close(CallBack _handle_close){
    handle_close = _handle_close;
}

void Channel::set_handle_read(CallBack _handle_read){
    handle_read = _handle_read;
}

void Channel::set_handle_write(CallBack _handle_write){
    handle_write = _handle_write;
}


