#include "listener.hpp"

static const unsigned int locReadSize = 8 * 1024;

Listener::Listener()
    : myId(1)
    , myAddressInfo(nullptr)
    , myPort("8080")
    , myConnectionQueueSize(30)
    , mySocket(-1)
{
}

Listener::~Listener()
{
}

void Listener::Start()
{
    PrivSetAddressInfo();
    PrivCreateSocket();

    PrivSetSocketReusable();
    PrivSetSocketNoDelay();
    PrivSetSocketBind();
    PrivSetSocketNonBlocking();
    PrivSetSocketListen();

    PrivCreateEpoll();

    std::cout << "[Listener] Started" << std::endl;
    
    while(true)
    {
        PrivHandleEvents();
    }
}

void Listener::SetCallback(
    RequestCallback     aCallback)
{
    myCallback = aCallback;
}

void Listener::PrivHandleEvents()
{
    const int event_count = epoll_wait(myEpoll, &myEpollEvents[0], myConnectionQueueSize, -1);
    for(int i = 0; i < event_count; ++i)
    {
        PrivHandleEvent(myEpollEvents[i]);
    }
}

void Listener::PrivReadData(
    const int           aSocket,
    std::string&        aOutData)
{
    char buffer[locReadSize];
    memset(buffer, 0, locReadSize);

    read(aSocket, buffer, locReadSize - 1);
    
    aOutData = buffer;
}

void Listener::PrivHandleEvent(
    const epoll_event&  aEvent)
{
    if((aEvent.events & EPOLLERR) ||
       (aEvent.events & EPOLLHUP) ||
       (!(aEvent.events & EPOLLIN)))
    {
        close(aEvent.data.fd);

        return;
    }

    if(mySocket == aEvent.data.fd)
    {
        while(true)
        {
            sockaddr_in cli_addr;
            socklen_t clilen = sizeof(cli_addr);

            const int newsockfd = accept(mySocket, (sockaddr*)&cli_addr, &clilen);
            if(newsockfd == -1)
            {
                break;
            }
            
            PrivSetEpollInterface(newsockfd);
            mySocketAddress[newsockfd] = cli_addr.sin_addr;
        }
        
        return;
    }
    
    const int newsockfd = aEvent.data.fd;
    
    std::string data = "";
    PrivReadData(newsockfd, data);
    
    const std::string ip_address = inet_ntoa(mySocketAddress[newsockfd]);
    
    myCallback(newsockfd, data, ip_address);
}

void Listener::PrivSetAddressInfo()
{
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, myPort.c_str(), &hints, &myAddressInfo);
}

void Listener::PrivCreateSocket()
{
    mySocket = socket(myAddressInfo->ai_family,
                      myAddressInfo->ai_socktype,
                      myAddressInfo->ai_protocol);
}

void Listener::PrivSetSocketReusable()
{
    const int yes = 1;
    setsockopt(mySocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
}

void Listener::PrivSetSocketNoDelay()
{
    const int yes = 1;
    setsockopt(mySocket, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(int));
}

void Listener::PrivSetSocketBind()
{
    bind(mySocket, myAddressInfo->ai_addr, myAddressInfo->ai_addrlen);
}

void Listener::PrivSetSocketNonBlocking()
{
    int flags = fcntl(mySocket, F_GETFL, 0);
    flags |= O_NONBLOCK;

    fcntl(mySocket, F_SETFL, flags);
}

void Listener::PrivSetSocketListen()
{
    listen(mySocket, myConnectionQueueSize);
}

void Listener::PrivCreateEpoll()
{
    myEpoll = epoll_create1(0);

    PrivSetEpollInterface(mySocket);

    myEpollEvents.resize(myConnectionQueueSize);
}

void Listener::PrivSetEpollInterface(
    const int   aSocket)
{
    epoll_event event;
    event.data.fd = aSocket;
    event.events = EPOLLIN | EPOLLET;

    epoll_ctl(myEpoll, EPOLL_CTL_ADD, aSocket, &event);
}
