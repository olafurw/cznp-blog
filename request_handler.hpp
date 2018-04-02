#ifndef REQUEST_HANDLER
#define REQUEST_HANDLER

#include <string>

#include "request.hpp"

class RequestHandler
{
public:
                        RequestHandler();
                        ~RequestHandler();
    
    void                OnRequest(
                            const int       aSocket,
                            const Request   aRequest);

private:

};

#endif
