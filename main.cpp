#include <iostream>
#include <thread>
#include <memory>

#include "http_parser.hpp"
#include "listener.hpp"
#include "request_handler.hpp"

static void
ListenerRunner()
{
    Listener listener;
    RequestHandler requestHandler;

    listener.SetCallback(
        [&](const int aSocket, const std::string& aData, const std::string& ipAddress)
        {
            HttpParser parser(aData);
            if (parser.IsParsingError())
            {
                return;
            }

            requestHandler.OnRequest(aSocket, parser.GetRequest());
        }
    );

    listener.Start();
}

int
main(
    int argc,
    char** argv)
{
    std::thread listener_thread(ListenerRunner);
    listener_thread.join();
    
    return 0;
}
