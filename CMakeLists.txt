cmake_minimum_required(VERSION 3.10)
project(libdisc)

find_package(ZLIB REQUIRED)
find_package(CURL CONFIG REQUIRED)
find_package(Libwebsockets CONFIG REQUIRED)

add_library(libdisc SHARED src/disc.c src/disc_app.c src/disc_endpoint.c src/disc_json.c src/disc_gateway.c src/disc_objects.c)

target_link_libraries(libdisc PRIVATE ZLIB::ZLIB)
target_link_libraries(libdisc PRIVATE CURL::libcurl)
target_link_libraries(libdisc PRIVATE websockets_shared)
