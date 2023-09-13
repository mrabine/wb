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

**wb** [options] [http[s]://]hostname[:port]/path

**-c**\
&emsp;concurrency level\
**-h**\
&emsp;show available options\
**-k**\
&emsp;enable keep alive\
**-n requests**\
&emsp;number of requests to perform\
**-t**\
&emsp;request timeout in seconds\
**-v**\
&emsp;verbose\
**-V**\
&emsp;print version

## Example

```bash
wb -c 2 -n 1000 -k https://foo.bar/

benchmarking "foo.bar" on port 443 ...

Server Hostname:        foo.bar
Server Port:            443

Scheme:                 https
Document Path:          /

Concurrency Level:      2
Time taken for tests:   0.293823 seconds
Completed requests:     1000
Failed requests:        0
Requests per second:    3403.41 [#/sec]
```

## License

[MIT](https://choosealicense.com/licenses/mit/)

