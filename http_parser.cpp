#include "http_parser.hpp"

#include "utils.hpp"

HttpParser::HttpParser(
    const std::string&      aRequest)
    : myParsingError(false)
{
    auto requestLines = utils::split_string(aRequest, '\n', false, 1);

    if(requestLines.empty())
    {
        myParsingError = true;
        return;
    }

    if (!PrivParseHeader(requestLines[0]))
    {
        myParsingError = true;
        return;
    }

    myParsingError = false;
}

bool
HttpParser::IsParsingError() const
{
    return myParsingError;
}

Request
HttpParser::GetRequest() const
{
    if (myParsingError)
    {
        assert(false);
    }

    return Request(myMethod, myPath);
}

bool
HttpParser::PrivParseHeader(
    const std::string&      aRequestHeader)
{
    const auto lineExploded = utils::split_string(aRequestHeader, ' ', false, 3);

    if(lineExploded.size() < 3)
    {
        return false;
    }

    const std::string& method = lineExploded.at(0);

    if(method == "GET")
    {
        myMethod = RouteMethod_Get;
    }
    else
    {
        return false;
    }

    myPath = utils::trim(lineExploded.at(1));

    return true;
}
