## Description
An http get request was made to a url with C++. The code was developed in Linux (Debian 12) environment and compiled with cmake.
How to run the code and library requirements are available in the documentation file.


# Restbed Library

[![Build Status](https://travis-ci.org/yourusername/restbed.svg?branch=main)](https://travis-ci.org/yourusername/restbed)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Restbed is a flexible and extensible library for creating RESTful web services using modern C++. This library provides a wide range of features to develop web services quickly and efficiently.

## Features

- RESTful API support
- HTTPS support
- Asynchronous processing management
- Middleware support
- Cross-platform support (Windows, Linux, macOS)

## Installation

### Prerequisites

To use Restbed, you need to have the following requirements:

- A C++ compiler supporting C++11 or higher standards
- CMake (version 3.1 or newer)

### Build and Installation

To include Restbed in your project, follow these steps:

1. Clone the Restbed GitHub repository:

    ```bash
    git clone https://github.com/Corvusoft/restbed.git
    cd restbed
    ```

2. Build and install Restbed:

    ```bash
    mkdir build
    cd build
    cmake ..
    make
    sudo make install
    ```

3. Include the Restbed library in your project. For example, if you are using CMake:

    ```cmake
    cmake_minimum_required(VERSION 3.10)
    project(MyProject)

    set(CMAKE_CXX_STANDARD 11)

    find_package(Restbed REQUIRED)
    include_directories(${RESTBED_INCLUDE_DIRS})

    add_executable(MyProject main.cpp)
    target_link_libraries(MyProject ${RESTBED_LIBRARIES})
    ```

## Usage

A simple RESTful service example:

```cpp
#include <restbed>

using namespace std;
using namespace restbed;

void get_method_handler(const shared_ptr<Session> session)
{
    const auto request = session->get_request();
    int content_length = 0;
    request->get_header("Content-Length", content_length);

    session->close(OK, "Hello, World!", {{"Content-Length", "13"}});
}

int main(int argc, char* argv[])
{
    auto resource = make_shared<Resource>();
    resource->set_path("/hello");
    resource->set_method_handler("GET", get_method_handler);

    auto settings = make_shared<Settings>();
    settings->set_port(8080);

    Service service;
    service.publish(resource);
    service.start(settings);

    return 0;
}
