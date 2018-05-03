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
                            const int           aSocket,
                            const Request       aRequest);

private:
    void                PrivResponse200(
                            const int           aSocket,
                            const std::string&  aResponseString,
                            const time_t        aLastModified,
                            const std::string&  aFileMimeType);
    void                PrivResponse404(
                            const int           aSocket);
};

#endif
