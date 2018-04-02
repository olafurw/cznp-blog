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
    Request(): myMethod(RouteMethod_Unknown) { std::cout << "default ctor\n"; };
    Request(
        const RouteMethod   aMethod,
        const std::string&  aPath)
        : myMethod(aMethod)
        , myPath(aPath){ std::cout << "ctor\n"; }
    Request(
        const Request&  aRequest)
        : myMethod(aRequest.myMethod)
        , myPath(aRequest.myPath){ std::cout << "copy ctor\n"; }
    Request(
        Request&&       aRequest)
        : myMethod(aRequest.myMethod)
        , myPath(aRequest.myPath){ std::cout << "move ctor\n"; }

    const RouteMethod   myMethod;
    const std::string   myPath;
};

#endif
