# wb

[![Security Status](https://github.com/mrabine/wb/workflows/security/badge.svg)](https://github.com/mrabine/wb/security/code-scanning)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/96362326939a46bd8673b5b70f8654f7)](https://app.codacy.com/gh/mrabine/wb/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)
[![GitHub Releases](https://img.shields.io/github/release/mrabine/wb.svg)](https://github.com/mrabine/wb/releases/latest)
[![GitHub License](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/mrabine/wb/blob/main/LICENSE)

**web bench** is a web server benchmarking tool for debug purpose

## Download

To download the latest source do this:
```bash
git clone https://github.com/mrabine/wb.git
```

## Configuration

To configure **wb** do this:
```bash
cd wb
mkdir build && cd build
cmake ..
```

## Installation

To install **wb** do this:
```bash
make && sudo make install
```

## Usage

**wb** \[options] http\[s]://hostname\[:port]/path

**-c**\
&emsp;concurrency level (default: 1)\
**-h**\
&emsp;show available options\
**-H**\
&emsp;send HEAD request\
**-k**\
&emsp;enable keep alive\
**-n requests**\
&emsp;number of requests to perform (default: 1)\
**-P file**\
&emsp;file to POST (mime type is deduced from file extension)\
**-t**\
&emsp;request timeout in seconds\
**-U file**\
&emsp;file to PUT (mime type is deduced from file extension)\
**-v**\
&emsp;verbose\
**-V**\
&emsp;print version

## Example

```bash
wb -v https://joinframework.net/

benchmarking "joinframework.net" on port 443 ...

GET / HTTP/1.1
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: fr-FR,fr;q=0.8,en-US;q=0.6,en;q=0.4
Connection: close
Host: joinframework.net
If-Modified-Since: Sat, 1 Jan 2000 00:00:00 GMT
User-Agent: wb/1.0.0

HTTP/1.1 200 OK
Accept-Ranges: bytes
Connection: close
Content-Length: 1474
Content-Type: text/html
Date: Thu, 21 Sep 2023 10:03:59 GMT
ETag: "5c2-5c8786d2d515c"
Last-Modified: Sun, 01 Aug 2021 05:07:16 GMT
Vary: Accept-Encoding


Server Hostname:        joinframework.net
Server Port:            443

Scheme:                 https
Document Path:          /

Concurrency Level:      1
Time taken for tests:   0.0178217 seconds
Completed requests:     1
Failed requests:        0
Requests per second:    56.1113 [#/sec]
```

## License

[MIT](https://choosealicense.com/licenses/mit/)
