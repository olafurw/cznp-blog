#include "request_handler.hpp"

#include "libraries/format.h"
#include "libraries/json.hpp"

#include "utils.hpp"

#include <unistd.h>
#include <iostream>
#include <utility>
#include <optional>

using namespace nlohmann;

struct Note
{
    int             myId;
    std::string     myTitle;
    std::string     myDate;
};

static int
locParseNoteData(
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
locIsNoteExists(
    const std::string&  aPath)
{
    const int noteId = locParseNoteData(aPath);
    if (noteId < 1)
    {
        return false;
    }

    const std::string noteListJson = utils::file_to_string("../notes/list.json");

    try
    {
        const json noteList = json::parse(noteListJson);
        for(const auto& noteEntry : noteList)
        {
            if (noteEntry["id"].get<int>() == noteId)
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

static std::optional<Note>
locExtractNote(
    const std::string&  aPath)
{
    const int noteId = locParseNoteData(aPath);
    if (noteId < 1)
    {
        return {};
    }

    const std::string noteListJson = utils::file_to_string("../notes/list.json");

    try
    {
        const json noteList = json::parse(noteListJson);
        for(const auto& noteEntry : noteList)
        {
            if (noteEntry["id"].get<int>() == noteId)
            {
                return Note{noteId, noteEntry["title"], noteEntry["date"]};
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
locFindNoteData(
    const std::string&  aPath)
{
    const auto noteOptional = locExtractNote(aPath);
    if (!noteOptional)
    {
        return {};
    }

    const auto& note = noteOptional.value();

    json j;
    j["id"] = note.myId;
    j["title"] = note.myTitle;
    j["date"] = note.myDate;
    j["text"] = utils::file_to_string("../notes/" + std::to_string(note.myId) + ".txt");

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

    if (aRequest.myPath == "/"
        || aRequest.myPath == "/about"
        || aRequest.myPath == "/about/")
    {
        PrivResponse200(aSocket, utils::file_to_string("../www/index.html"), "text/html");
        return;
    }

    if (aRequest.myPath == "/about-data/")
    {
        PrivResponse200(aSocket, utils::file_to_string("../notes/about.txt"), "text/plain");
        return;
    }

    if (aRequest.myPath.size() >= 6 
		&& aRequest.myPath.substr(0, 6) == "/note/")
    {
        const auto exist = locIsNoteExists(aRequest.myPath);
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

    if (aRequest.myPath == "/note-list/")
    {
        PrivResponse200(aSocket, utils::file_to_string("../notes/list.json"), "application/json");
        return;
    }

    if (aRequest.myPath.size() >= 11
		&& aRequest.myPath.substr(0, 11) == "/note-data/")
    {
        const auto noteDataOptional = locFindNoteData(aRequest.myPath);
        if (!noteDataOptional)
        {
            PrivResponse404(aSocket);
            return;
        }

        PrivResponse200(aSocket, noteDataOptional.value(), "application/json");
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
