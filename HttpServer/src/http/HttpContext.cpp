#include "../../include/http/HttpContext.h"
#include <muduo/base/Logging.h>

using namespace muduo;
using namespace muduo::net;

namespace http
{

// 将报文解析出来将关键信息封装到HttpRequest对象里面去
bool HttpContext::parseRequest(Buffer *buf, Timestamp receiveTime)
{
    bool ok = true;
    bool hasMore = true;

    while (hasMore)
    {
        if (state_ == kExpectRequestLine)
        {
            const char *crlf = buf->findCRLF();
            if (crlf)
            {
                LOG_INFO << "[Parse] >> 请求行: "
                         << std::string(buf->peek(), crlf);
                ok = processRequestLine(buf->peek(), crlf);
                if (ok)
                {
                    request_.setReceiveTime(receiveTime);

                    if (crlf + 2 <= buf->beginWrite())
                    {
                        buf->retrieveUntil(crlf + 2);
                        state_ = kExpectHeaders;
                    }
                    else
                    {
                        LOG_WARN << "[Parse] 请求行未完整接收，等待更多数据";
                        hasMore = false;
                    }
                }
                else
                {
                    LOG_ERROR << "[Parse] !! 请求行格式错误";
                    hasMore = false;
                }
            }
            else
            {
                hasMore = false;
            }
        }
        else if (state_ == kExpectHeaders)
        {
            const char *crlf = buf->findCRLF();
            if (crlf)
            {
                const char *colon = std::find(buf->peek(), crlf, ':');
                if (colon < crlf)
                {
                    std::string header(buf->peek(), crlf);
                    LOG_INFO << "[Parse] >> Header: " << header;
                    request_.addHeader(buf->peek(), colon, crlf);
                }
                else if (strncmp(buf->peek(), "\r\n", 2) == 0)
                {
                    if (buf->readableBytes() < 2)
                    {
                        LOG_WARN << "[Parse] Header 结束标志不完整，等待更多数据";
                        hasMore = false;
                        break;
                    }
                    buf->retrieve(2); // 跳过空行

                    std::string contentLength = request_.getHeader("Content-Length");
                    LOG_INFO << "[Parse] >> 结束Header, Content-Length: " << contentLength;

                    if (request_.method() == HttpRequest::kPost ||
                        request_.method() == HttpRequest::kPut)
                    {
                        if (!contentLength.empty())
                        {
                            try {
                                int len = std::stoi(contentLength);
                                request_.setContentLength(len);
                                LOG_INFO << "[Parse] >> 设置 contentLength: " << len;

                                if (len > 0)
                                {
                                    state_ = kExpectBody;
                                }
                                else
                                {
                                    state_ = kGotAll;
                                    hasMore = false;
                                }
                            } catch (const std::exception &e) {
                                LOG_ERROR << "[Parse] !! Content-Length 无法解析: " << contentLength;
                                ok = false;
                                hasMore = false;
                            }
                        }
                        else
                        {
                            LOG_ERROR << "[Parse] !! POST 请求缺少 Content-Length";
                            ok = false;
                            hasMore = false;
                        }
                    }
                    else
                    {
                        state_ = kGotAll;
                        hasMore = false;
                    }
                    continue; // 跳过 retrieveUntil
                }
                else
                {
                    LOG_ERROR << "[Parse] !! Header 格式错误: "
                              << std::string(buf->peek(), crlf);
                    ok = false;
                    hasMore = false;
                }

                if (crlf + 2 <= buf->beginWrite())
                {
                    buf->retrieveUntil(crlf + 2);
                }
                else
                {
                    LOG_WARN << "[Parse] Header 行不完整，等待更多数据";
                    hasMore = false;
                }
            }
            else
            {
                hasMore = false;
            }
        }
        else if (state_ == kExpectBody)
        {
            size_t readable = buf->readableBytes();
            int contentLen = request_.contentLength();

            LOG_INFO << "[Parse] >> 开始读取 Body, 已收: " << readable
                     << ", 目标长度: " << contentLen;

            if (readable < static_cast<size_t>(contentLen))
            {
                LOG_INFO << "[Parse] >> Body 数据未接收完，等待更多数据";
                hasMore = false;
                return true;
            }

            std::string body(buf->peek(), buf->peek() + contentLen);
            request_.setBody(body);
            buf->retrieve(contentLen);
            LOG_INFO << "[Parse] >> 完整接收 Body: " << body;

            state_ = kGotAll;
            hasMore = false;
        }
    }

    return ok;
}



// 解析请求行
bool HttpContext::processRequestLine(const char *begin, const char *end)
{
    bool succeed = false;
    const char *start = begin;
    const char *space = std::find(start, end, ' ');
    if (space != end && request_.setMethod(start, space))
    {
        start = space + 1;
        space = std::find(start, end, ' ');
        if (space != end)
        {
            const char *argumentStart = std::find(start, space, '?');
            if (argumentStart != space) // 请求带参数
            {
                request_.setPath(start, argumentStart); // 注意这些返回值边界
                request_.setQueryParameters(argumentStart + 1, space);
            }
            else // 请求不带参数
            {
                request_.setPath(start, space);
            }

            start = space + 1;
            succeed = ((end - start == 8) && std::equal(start, end - 1, "HTTP/1."));
            if (succeed)
            {
                if (*(end - 1) == '1')
                {
                    request_.setVersion("HTTP/1.1");
                }
                else if (*(end - 1) == '0')
                {
                    request_.setVersion("HTTP/1.0");
                }
                else
                {
                    succeed = false;
                }
            }
        }
    }
    return succeed;
}

} // namespace http