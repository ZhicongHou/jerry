#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include <errno.h>
#include <iostream>
#include <pthread.h>
#include <sys/epoll.h>

using namespace std;

/**
 * 1.创建socket
 * 2.配置地址
 * 3.connect连接
 * 4.循环发信息
 * */

int main(){


    // 创建socket
    int sock_fd = socket(AF_INET,SOCK_STREAM,0);
    if(sock_fd==-1){
        perror("socket");
        exit(-1);
    }
    // printf("创建socket成功！\n");

    // 设置服务器（对方）的IP和端口
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    // inet_pton(AF_INET,"119.23.108.241",&addr.sin_addr.s_addr);
    addr.sin_port = htons(6970);

    // 连接
    int ret = connect(sock_fd, (struct sockaddr *)&addr, sizeof(addr));
    if(ret==-1){
        perror("connect");
        exit(-1);
    }
    printf("connect成功！\n");



    // 创建epoll
    int ep_fd = epoll_create(100);
    // 将sock_fd服务器监听的文件描述符放到epoll中
    epoll_event epev;
    epev.events = EPOLLOUT|EPOLLHUP|EPOLLRDHUP;
    epev.data.fd = sock_fd;
    epoll_ctl(ep_fd, EPOLL_CTL_ADD, sock_fd, &epev);


    // 写信息
    char write_buff[1024];
    struct epoll_event epevs[1024];

    while(true){

        int n = epoll_wait(ep_fd, epevs, 1024, -1);
        if(n==-1){
            perror("epoll_wait");
            exit(-1);
        }

        for(int i = 0; i<n; i++){
            if(epevs[i].data.fd==sock_fd){
                 //监听服务器是否已经关闭。现在还没做到连接断开时实时关闭client，能否用信号sig来实现实时？
                if(epevs[i].events&EPOLLHUP || epevs[i].events&EPOLLRDHUP){ 
                    printf("服务器已经关闭连接！\n");
                    exit(-1);
                }else if(epevs[i].events&EPOLLOUT){ //监听服务器是否可写
                    cout<<"输入信息：";
                    cin.getline(write_buff, sizeof(write_buff));
                    if(strlen(write_buff)==0){
                        continue;
                    }
                    ssize_t len = write(sock_fd, write_buff, strlen(write_buff)+1);
                    if(len==-1){
                        perror("write");
                        exit(-1);
                    }
                }
            }
        }
        
    }
    close(sock_fd);
}
