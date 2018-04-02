#ifndef HTTP_PARSER
#define HTTP_PARSER

#include <string>
#include <vector>
#include <memory>
#include <map>

#include "request.hpp"

class HttpParser
{
public:
                        HttpParser(
                            const std::string&      aRequest);

    bool                IsParsingError() const;

    Request             GetRequest() const;

private:
    bool                PrivParseHeader(
                            const std::string&      aRequestHeader);

    RouteMethod         myMethod;
    std::string         myPath;
    bool                myParsingError;
};

#endif
