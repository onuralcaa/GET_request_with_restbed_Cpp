cmake_minimum_required(VERSION 3.10)

# Project name
project(send_request)

# C++ standardını belirleyin
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(send_request src/main.cpp)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

set(RESTBED_ROOT "/home/onur/Masaüstü/send_request/restbed")

include_directories(${RESTBED_ROOT}/distribution/include)
link_directories(${RESTBED_ROOT}/distribution/library)

target_link_libraries(send_request
    ${RESTBED_ROOT}/distribution/library/librestbed.a
    ${CMAKE_THREAD_LIBS_INIT}
    OpenSSL::SSL
    OpenSSL::Crypto
)

target_link_libraries(send_request
    -lpthread
    -lssl
    -lcrypto
)
