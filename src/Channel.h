

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



class Channel{


private:
    typedef std::function<void()> CallBack;

public:
    Channel();
    ~Channel();

    void handle(uint32_t events);
    CallBack handle_close;
    CallBack handle_read;
    CallBack handle_write;
    void set_handle_close(CallBack _handle_close);
    void set_handle_read(CallBack _handle_read);
    void set_handle_write(CallBack _handle_write);
};


