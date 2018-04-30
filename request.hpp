#ifndef REQUEST
#define REQUEST

#include <string>
#include <iostream>

enum RouteMethod
{
    RouteMethod_Unknown = 0,
    RouteMethod_Get
};

struct Request
{
    Request(): myMethod(RouteMethod_Unknown) { };
    Request(
        const RouteMethod   aMethod,
        const std::string&  aPath)
        : myMethod(aMethod)
        , myPath(aPath){ }
    Request(
        const Request&  aRequest)
        : myMethod(aRequest.myMethod)
        , myPath(aRequest.myPath){ }
    Request(
        Request&&       aRequest)
        : myMethod(aRequest.myMethod)
        , myPath(aRequest.myPath){ }

    const RouteMethod   myMethod;
    const std::string   myPath;
};

#endif
