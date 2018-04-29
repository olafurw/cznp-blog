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
            if (blogEntry["id"].get<int>() == blogId)
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
            if (blogEntry["id"].get<int>() == blogId)
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

static std::optional<std::string>
locFindBlogData(
    const std::string&  aPath)
{
    const auto blogOptional = locExtractBlog(aPath);
    if (!blogOptional)
    {
        return {};
    }

    const auto& blog = blogOptional.value();

    json j;
    j["id"] = blog.myId;
    j["title"] = blog.myTitle;
    j["date"] = blog.myDate;
    j["text"] = utils::file_to_string("../blog/" + std::to_string(blog.myId) + ".txt");

    return j.dump();
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
        PrivResponse200(aSocket, utils::file_to_string("../www/index.html"), "text/html");
        return;
    }

    if (aRequest.myPath.size() >= 6 
		&& aRequest.myPath.substr(0, 6) == "/blog/")
    {
        const auto exist = locIsBlogExists(aRequest.myPath);
        if (!exist)
        {
            PrivResponse404(aSocket);
            return;
        }

        PrivResponse200(aSocket, utils::file_to_string("../www/index.html"), "text/html");
        return;
    }

    if (aRequest.myPath == "/main.css")
    {
        PrivResponse200(aSocket, utils::file_to_string("../www/main.css"), "text/css");
        return;
    }

    if (aRequest.myPath == "/dist/build.js")
    {
        PrivResponse200(aSocket, utils::file_to_string("../www/dist/build.js"), "application/javascript");
        return;
    }

    if (aRequest.myPath == "/blog-list/")
    {
        PrivResponse200(aSocket, utils::file_to_string("../blog/list.json"), "application/json");
        return;
    }

    if (aRequest.myPath.size() >= 11
		&& aRequest.myPath.substr(0, 11) == "/blog-data/")
    {
        const auto blogDataOptional = locFindBlogData(aRequest.myPath);
        if (!blogDataOptional)
        {
            PrivResponse404(aSocket);
            return;
        }

        PrivResponse200(aSocket, blogDataOptional.value(), "application/json");
        return;
    }

    if (aRequest.myPath == "/images/logo.png")
    {
        PrivResponse200(aSocket, utils::file_to_string("../www/images/logo.png"), "image/png");
        return;
    }

    PrivResponse404(aSocket);
}

void
RequestHandler::PrivResponse200(
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

void
RequestHandler::PrivResponse404(
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
