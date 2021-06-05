
#include "Reactor.h"
#include "ThreadEventLoopPool.h"
#include "Epoll.h"
#include "Channel.h"
#include "Connection.h"

Reactor::Reactor(){
    thread_pool = new ThreadPool(1,1,1,1);
    epoll = new Epoll();
    main_channel = new Channel();
}
Reactor::~Reactor(){
    delete thread_pool;
    delete epoll;
}


void Reactor::init_socket(){
// 创建socket
    sock_fd = socket(AF_INET,SOCK_STREAM,0);
    if(sock_fd==-1){
        perror("socket");
        exit(-1);
    }


    // 设置端口复用
    int optval = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)); 
    // setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));    

    // 配置服务器的IP地址和监听的端口
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    // inet_pton(AF_INET,"0.0.0.0",&addr.sin_addr.s_addr);
    addr.sin_port = htons(6970);
    // 绑定文件描述符fd和IP地址&端口
    int ret = bind(sock_fd,(struct sockaddr *)&addr,sizeof(addr));
    if(ret==-1){
        perror("bind");
        exit(-1);
    }

    // 开启监听
    ret = listen(sock_fd, 3);
    if(ret==-1){
        perror("listen");
        exit(-1);
    }
    printf("正在监听……\n");
}

void Reactor::init (){
    // 初始化线程池
    thread_pool->init();
    init_socket();
    // 将sock放到epoll中监听
    epoll->add(sock_fd);
    main_channel->set_handle_read(std::bind(&Reactor::handle_accept,this));
}

void Reactor::run(){
    struct epoll_event *epevs;
    while(true){
        int n = epoll->wait();
        epevs = epoll->epevs;
        for(int i = 0; i<n; i++){
            int cur_fd = epevs[i].data.fd;
            if(cur_fd==sock_fd){  // 有新连接
                main_channel->handle(epevs[i].events);     
            }
        }
    }
}


void Reactor::handle_accept(){
    // 接收新连接
    struct sockaddr_in cli_addr;
    uint32_t addr_len = sizeof(cli_addr);
    int cli_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &addr_len);
    if(cli_fd==-1){
        perror("accept");
        exit(-1);
    }
    int ip_len = 16;
    char *ip = new char[ip_len];    // 要在Channel的析构函数里面delete
    inet_ntop(AF_INET6, &cli_addr.sin_addr.s_addr, ip, ip_len);
    uint16_t port = ntohs(cli_addr.sin_port);
    printf("新连接IP为：%s，Port为：%u fd:%d\n", ip, port,cli_fd);
    // 要在remove的时候delete ip
    Connection *connection = new Connection(cli_fd, ip, port); 
    thread_pool->add_connection(connection);   
}


int main()
{
    Reactor reactor;
    reactor.init();
    reactor.run();
}






