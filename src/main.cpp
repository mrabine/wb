/**
 * MIT License
 *
 * Copyright (c) 2023 Mathieu Rabine
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// wb.
#include "version.hpp"

// libjoin.
#include <join/httpclient.hpp>
#include <join/filesystem.hpp>
#include <join/utils.hpp>

// C++.
#include <filesystem>
#include <fstream>
#include <future>

using join::Resolver;
using join::HttpMethod;
using join::HttpRequest;
using join::HttpResponse;
using join::Https;
using join::Http;

std::atomic <int> nreq  = 0;
std::atomic <int> nfail = 0;

// =========================================================================
//   CLASS     :
//   METHOD    : version
// =========================================================================
void version ()
{
    std::cout << BINARY_NAME << " version " << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << std::endl;
}

// =========================================================================
//   CLASS     :
//   METHOD    : usage
// =========================================================================
void usage ()
{
    std::cout << "Usage" << std::endl;
    std::cout << "  " << BINARY_NAME << " [options] http[s]://hostname[:port]/path" << std::endl;
    std::cout << std::endl;
    std::cout << "Options" << std::endl;
    std::cout << "  -c                concurrency level (default: 1)" << std::endl;
    std::cout << "  -h                show available options" << std::endl;
    std::cout << "  -H                send HEAD request" << std::endl;
    std::cout << "  -K                enable keep alive" << std::endl;
    std::cout << "  -n requests       number of requests to perform (default: 1)" << std::endl;
    std::cout << "  -P file           file to POST (mime type is deduced from file extension)" << std::endl;
    std::cout << "  -t                request timeout in seconds" << std::endl;
    std::cout << "  -U file           file to PUT (mime type is deduced from file extension)" << std::endl;
    std::cout << "  -v                verbose" << std::endl;
    std::cout << "  -V                print version" << std::endl;
}

// =========================================================================
//   CLASS     :
//   METHOD    : benchmark
// =========================================================================
template <class Client>
void benchmark (const std::string& host, uint16_t port, HttpRequest request, const std::string& file, int timeout, int max, bool verbose)
{
    Client client (host, port, false);
    client.timeout (timeout * 1000);

    while (nreq++ < max)
    {
        HttpResponse response;

        // send request headers.
        if (client.send (request) == -1)
        {
            ++nfail;
            client.disconnect ();
            client.close ();
            continue;
        }

        if (verbose)
        {
            std::cout << request.methodString () << " " << request.urn () << " " << request.version () << std::endl;
            std::cout << request.dumpHeaders ();
        }

        // send payload.
        std::fstream fi (file);
        if (fi.is_open ())
        {
            client << fi.rdbuf ();
            client.flush ();
            fi.close ();
        }

        // read response headers.
        if (client.receive (response) == -1)
        {
            ++nfail;
            client.disconnect ();
            client.close ();
            continue;
        }

        if (verbose)
        {
            std::cout << response.version () << " " << response.status () << " " << response.reason () << std::endl;
            std::cout << response.dumpHeaders ();
        }

        // read payload.
        if (response.contentLength ())
        {
            std::string payload;
            payload.resize (response.contentLength ());
            client.read (&payload[0], payload.size ());
        }
        else if (response.header ("Transfert-Encoding").find ("chunked") != std::string::npos)
        {
            std::string payload;
            payload.resize (4096);
            while (client.read (&payload[0], payload.size ())) {}
            client.clear ();
        }
    }

    --nreq;
}

// =========================================================================
//   CLASS     :
//   METHOD    : main
// =========================================================================
int main (int argc, char *argv[])
{
    HttpRequest request;
    int tasks = 1, max = 1, timeout = 5;
    bool verbose = false;
    std::string file;

    int opt;
    while ((opt = getopt (argc, argv, "c:hHKn:P:t:U:vV")) != -1)
    {
        switch (opt)
        {
            case 'c':
                tasks = std::stoi (optarg);
                break;
            case 'h':
                usage ();
                _exit (EXIT_SUCCESS);
            case 'H':
                request.method (HttpMethod::Head);
                break;
            case 'K':
                request.header ("Connection", "keep-alive");
                break;
            case 'n':
                max = std::stoi (optarg);
            break;
            case 'P':
                file = optarg;
                request.method (HttpMethod::Post);
                request.header ("Content-Type", join::mime (file));
                request.header ("Content-Length", std::to_string (std::filesystem::file_size (file)));
                break;
            case 't':
                timeout = std::stoi (optarg);
            break;
            case 'U':
                file = optarg;
                request.method (HttpMethod::Put);
                request.header ("Content-Type", join::mime (file));
                request.header ("Content-Length", std::to_string (std::filesystem::file_size (file)));
                break;
            case 'v':
                verbose = true;
            break;
            case 'V':
                version ();
                _exit (EXIT_SUCCESS);
            default:
                usage ();
                _exit (EXIT_FAILURE);
        }
    }

    if (optind != argc - 1)
    {
        std::cout << "invalid arguments" << std::endl;
        std::cout << std::endl;
        usage ();
        _exit (EXIT_FAILURE);
    }

    // regular expression inspired by rfc3986 (see https://www.ietf.org/rfc/rfc3986.txt)
    // ex.
    // 0: https://example.com:8080/foo/bar.html?val=1#frag  # URL
    // 1: https                                             # Scheme
    // 2: example.com                                       # Host
    // 3: 8080                                              # Port
    // 4: /foo/bar.html                                     # Path
    // 5: val=1                                             # Query
    // 6: frag                                              # Fragment
    //                0    1              2                                    3         4            5            6
    std::regex reg (R"(^(?:([^:/?#]+)://)?([a-z0-9\-._~%]+|\[[a-f0-9:.]+\])(?::([0-9]+))?([^?#]*)(?:\?([^#]*))?(?:#(.*))?)");
    std::cmatch match;

    if (!std::regex_match (argv[optind], match, reg))
    {
        std::cout << "invalid arguments" << std::endl;
        std::cout << std::endl;
        usage ();
        _exit (EXIT_FAILURE);
    }

    std::string scheme = match[1];
    std::string host   = match[2];
    uint16_t    port   = match[3].length () ? uint16_t (std::stoi (match[3])) : Resolver::resolveService (scheme);
    std::string path   = match[4];

    request.path   (path);
    request.header ("Accept-Language", "fr-FR,fr;q=0.8,en-US;q=0.6,en;q=0.4");
    request.header ("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
    request.header ("If-Modified-Since", "Sat, 1 Jan 2000 00:00:00 GMT");
    request.header ("User-Agent", BINARY_NAME "/" VERSION_MAJOR "." VERSION_MINOR "." VERSION_PATCH);

    std::cout << std::endl;
    std::cout << "benchmarking \"" << host << "\" on port " << port << " ..." << std::endl;
    std::cout << std::endl;

    std::vector <std::future <void>> futures;

    auto beg = std::chrono::high_resolution_clock::now ();

    for (int i = 0; i < tasks; ++i)
    {
        if (scheme == "https")
        {
            futures.push_back (std::async (std::launch::async, benchmark <Https::Client>, host, port, request, file, timeout, max, verbose));
        }
        else
        {
            futures.push_back (std::async (std::launch::async, benchmark <Http::Client>, host, port, request, file, timeout, max, verbose));
        }
    }

    for (auto& future : futures)
    {
        future.get ();
    }

    auto end = std::chrono::high_resolution_clock::now ();

    std::chrono::duration <double> elapsed = end - beg;

    std::cout << std::endl;
    std::cout << "Server Hostname:        " << host << std::endl;
    std::cout << "Server Port:            " << port << std::endl;
    std::cout << std::endl;
    std::cout << "Scheme:                 " << scheme << std::endl;
    std::cout << "Document Path:          " << request.path () << std::endl;
    std::cout << std::endl;
    std::cout << "Concurrency Level:      " << tasks << std::endl;
    std::cout << "Time taken for tests:   " << elapsed.count () << " seconds" << std::endl;
    std::cout << "Completed requests:     " << nreq - nfail << std::endl;
    std::cout << "Failed requests:        " << nfail << std::endl;
    std::cout << "Requests per second:    " << nreq / elapsed.count () << " [#/sec]" << std::endl;
    std::cout << std::endl;

    _exit ( EXIT_SUCCESS );
}
