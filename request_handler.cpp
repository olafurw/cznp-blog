#include "request_handler.hpp"

#include "libraries/format.h"
#include "libraries/json.hpp"

#include "utils.hpp"

#include <unistd.h>
#include <iostream>
#include <utility>
#include <optional>

using namespace nlohmann;

struct Blog
{
    int             myId;
    std::string     myTitle;
    std::string     myDate;
};

static int
locParseBlogData(
    const std::string&  aPath)
{
    auto pathSplit = utils::split_string(aPath, '/', false, 3);

    if (pathSplit.size() < 3)
    {
        return -1;
    }

    try
    {
        return std::stoi(pathSplit[2]);
    }
    catch(...)
    {
        return -1;
    }
}

static bool
locIsBlogExists(
    const std::string&  aPath)
{
    const int blogId = locParseBlogData(aPath);
    if (blogId < 1)
    {
        return false;
    }

    const std::string blogListJson = utils::file_to_string("../blog/list.json");

    try
    {
        const json blogList = json::parse(blogListJson);
        for(const auto& blogEntry : blogList)
        {
            if (blogEntry["id"] == blogId)
            {
                return true;
            }
        }
    }
    catch(...)
    {
        return false;
    }

    return false;
}

static std::optional<Blog>
locExtractBlog(
    const std::string&  aPath)
{
    const int blogId = locParseBlogData(aPath);
    if (blogId < 1)
    {
        return {};
    }

    const std::string blogListJson = utils::file_to_string("../blog/list.json");

    try
    {
        const json blogList = json::parse(blogListJson);
        for(const auto& blogEntry : blogList)
        {
            if (blogEntry["id"] == blogId)
            {
                return Blog{blogId, blogEntry["title"], blogEntry["date"]};
            }
        }
    }
    catch(...)
    {
        return {};
    }

    return {};
}

static void
locHttp200Response(
    const int           aSocket,
    const std::string&  aResponseString,
    const std::string&  aFileMimeType)
{
    static const std::string header = R"header(HTTP/1.0 200 OK
Server: cznp_server
MIME-version: 1.0
Content-type: {0}
Last-Modified: Thu, 1 Jan 1970 00:00:00 GMT
Content-Length: {1}

{2}

)header";
    
    const std::string fileData = fmt::format(header, aFileMimeType, aResponseString.size(), aResponseString);

    write(aSocket, fileData.c_str(), fileData.size());
    close(aSocket);
}

static void
locHttp404Response(
    const int           aSocket)
{
    static const std::string header = R"header(HTTP/1.0 404 Not Found
Server: cznp_server
MIME-version: 1.0
Content-type: text/plain
Last-Modified: Thu, 1 Jan 1970 00:00:00 GMT
Content-Length: 3

404

)header";

    write(aSocket, header.c_str(), header.size());
    close(aSocket);
}

RequestHandler::RequestHandler()
{

}

RequestHandler::~RequestHandler()
{

}

void
RequestHandler::OnRequest(
    const int       aSocket,
    const Request   aRequest)
{
    std::cout << aRequest.myPath << std::endl;

    if (aRequest.myPath == "/")
    {
        locHttp200Response(aSocket, utils::file_to_string("../www/index.html"), "text/html");
        return;
    }

    if (aRequest.myPath.substr(0, 6) == "/blog/")
    {
        const auto exist = locIsBlogExists(aRequest.myPath);
        if (!exist)
        {
            locHttp404Response(aSocket);
            return;
        }

        locHttp200Response(aSocket, utils::file_to_string("../www/index.html"), "text/html");
        return;
    }

    if (aRequest.myPath == "/main.css")
    {
        locHttp200Response(aSocket, utils::file_to_string("../www/main.css"), "text/css");
        return;
    }

    if (aRequest.myPath == "/dist/build.js")
    {
        locHttp200Response(aSocket, utils::file_to_string("../www/dist/build.js"), "application/javascript");
        return;
    }

    if (aRequest.myPath == "/blog-list/")
    {
        locHttp200Response(aSocket, utils::file_to_string("../blog/list.json"), "application/json");
        return;
    }

    if (aRequest.myPath.substr(0, 11) == "/blog-data/")
    {
        const auto blogOptional = locExtractBlog(aRequest.myPath);
        if (!blogOptional)
        {
            locHttp404Response(aSocket);
            return;
        }

        const auto& blog = blogOptional.value();

        json j;
        j["id"] = blog.myId;
        j["title"] = blog.myTitle;
        j["date"] = blog.myDate;
        j["text"] = utils::file_to_string("../blog/" + std::to_string(blog.myId) + ".txt");

        locHttp200Response(aSocket, j.dump(), "application/json");
        return;
    }

    locHttp404Response(aSocket);
}