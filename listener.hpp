#ifndef SERV_LISTENER
#define SERV_LISTENER

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <net/route.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <memory>
#include <map>
#include <vector>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <iostream>

typedef std::function<void(const int, const std::string&, const std::string&)> RequestCallback;

class Listener
{
public:
                        Listener();
                        ~Listener();
    
    void                Start();
    
    void                SetCallback(
                            RequestCallback             aCallback);

private:
    void                PrivHandleEvents();
    void                PrivHandleEvent(
                            const epoll_event&          aEvent);
    
    void                PrivReadData(
                            const int                   aSocket,
                            std::string&                aOutData);
    
    void                PrivSetAddressInfo();
    void                PrivCreateSocket();

    void                PrivSetSocketReusable();
    void                PrivSetSocketNonBlocking();
    void                PrivSetSocketBind();
    void                PrivSetSocketNoDelay();
    void                PrivSetSocketListen();

    void                PrivCreateEpoll();
    void                PrivSetEpollInterface(
                           const int                    aSocket);
    
    std::string                         myPort;
    addrinfo*                           myAddressInfo;
    std::vector<epoll_event>            myEpollEvents;
    int                                 myId;
    int                                 mySocket;
    int                                 myEpoll;
    int                                 myConnectionQueueSize;
    std::map<int, in_addr>              mySocketAddress;
    RequestCallback                     myCallback;
};

#endif
