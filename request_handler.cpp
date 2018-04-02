#include "request_handler.hpp"

#include "libraries/format.h"
#include "libraries/json.hpp"

#include "utils.hpp"

#include <unistd.h>
#include <iostream>
#include <utility>
#include <optional>

using namespace nlohmann;

static std::string
locHttpResponse200(
    const std::string&      aData,
    const std::string&      aContentType)
{
    static const std::string header = R"header(HTTP/1.0 200 OK
Server: cznp_server
MIME-version: 1.0
Content-type: {0}
Last-Modified: Thu, 1 Jan 1970 00:00:00 GMT
Content-Length: {1}

{2}

)header";
    
    return fmt::format(header, aContentType, aData.size(), aData);
}

static std::string
locHttpResponse404()
{
    static const std::string header = R"header(HTTP/1.0 404 Not Found
Server: cznp_server
MIME-version: 1.0
Content-type: text/plain
Last-Modified: Thu, 1 Jan 1970 00:00:00 GMT
Content-Length: 2

no

)header";
    
    return header;
}

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

static std::optional<std::pair<std::string, std::string>>
locExtractBlog(
    const int aBlogId)
{
    std::optional<std::pair<std::string, std::string>> result;

    const std::string blogListData = utils::file_to_string("../blog/list.json");

    try
    {
        const json blogList = json::parse(blogListData);

        for(const auto& blogEntry : blogList)
        {
            if (blogEntry["id"] == aBlogId)
            {
                return std::make_pair(blogEntry["title"], blogEntry["date"]);
            }
        }
    }
    catch(...)
    {
        return result;
    }

    return result;
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
        const std::string fileData = locHttpResponse200(utils::file_to_string("../www/index.html"), "text/html");

        write(aSocket, fileData.c_str(), fileData.size());
        close(aSocket);

        return;
    }

    if (aRequest.myPath == "/main.css")
    {
        const std::string fileData = locHttpResponse200(utils::file_to_string("../www/main.css"), "text/css");

        write(aSocket, fileData.c_str(), fileData.size());
        close(aSocket);

        return;
    }

   if (aRequest.myPath == "/dist/build.js")
    {
        const std::string fileData = locHttpResponse200(utils::file_to_string("../www/dist/build.js"), "application/javascript");

        write(aSocket, fileData.c_str(), fileData.size());
        close(aSocket);

        return;
    }

    if (aRequest.myPath == "/blog-list/")
    {
        const std::string blogList = locHttpResponse200(utils::file_to_string("../blog/list.json"), "application/json");

        write(aSocket, blogList.c_str(), blogList.size());
        close(aSocket);

        return;
    }

    if (aRequest.myPath.substr(0, 11) == "/blog-data/")
    {
        const int blogId = locParseBlogData(aRequest.myPath);
        if (blogId < 1)
        {
            const std::string notFound = locHttpResponse404();

            write(aSocket, notFound.c_str(), notFound.size());
            close(aSocket);

            return;
        }

        const auto blogDataOptional = locExtractBlog(blogId);
        if (!blogDataOptional)
        {
            const std::string notFound = locHttpResponse404();

            write(aSocket, notFound.c_str(), notFound.size());
            close(aSocket);

            return;
        }

        const auto& blogData = blogDataOptional.value();

        json j;
        j["id"] = blogId;
        j["title"] = blogData.first;
        j["date"] = blogData.second;
        j["text"] = utils::file_to_string("../blog/" + std::to_string(blogId) + ".txt");

        const std::string blogResponse = locHttpResponse200(j.dump(), "application/json");
        write(aSocket, blogResponse.c_str(), blogResponse.size());
        close(aSocket);

        return;
    }

    const std::string notFound = locHttpResponse404();

    write(aSocket, notFound.c_str(), notFound.size());
    close(aSocket);
}