cmake_minimum_required(VERSION 3.9.6)
project(libatp)

include_directories(
    /home/pengwang/cloud_platform/framework/3rd
    /home/pengwang/cloud_platform/project/libatp/src
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0 -pg -pipe -Wall -g -W")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fstrict-aliasing -Wno-invalid-offsetof -Wno-unused-parameter -fno-omit-frame-pointer -Wno-unused-function")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -luuid -lglog")

set(BUTIL_SOURCES
    ${PROJECT_SOURCE_DIR}/src/atp_socket.cpp
    ${PROJECT_SOURCE_DIR}/src/atp_dynamic_thread_pool.cpp
    ${PROJECT_SOURCE_DIR}/src/atp_libevent.cpp
    ${PROJECT_SOURCE_DIR}/src/atp_channel.cpp
    ${PROJECT_SOURCE_DIR}/src/atp_tcp_conn.cpp
    ${PROJECT_SOURCE_DIR}/src/atp_listener.cpp
    ${PROJECT_SOURCE_DIR}/src/atp_memory_pool.cpp
    ${PROJECT_SOURCE_DIR}/src/atp_memory_policy.cpp
    ${PROJECT_SOURCE_DIR}/src/atp_event_watcher.cpp
    ${PROJECT_SOURCE_DIR}/src/atp_event_loop.cpp
    ${PROJECT_SOURCE_DIR}/src/atp_event_loop_thread_pool.cpp
    ${PROJECT_SOURCE_DIR}/src/atp_tcp_server.cpp

    # benchmark
    #${PROJECT_SOURCE_DIR}/examples/atp_buffer_benchmark.cpp
    ${PROJECT_SOURCE_DIR}/examples/atp_listener_benchmark.cpp
    #${PROJECT_SOURCE_DIR}/examples/atp_memory_pool_benchmark.cpp

    #${PROJECT_SOURCE_DIR}/beast/socket.cpp
    #${PROJECT_SOURCE_DIR}/beast/buffer.cpp
    #${PROJECT_SOURCE_DIR}/beast/channel.cpp
    #${PROJECT_SOURCE_DIR}/beast/policy.cpp
    #${PROJECT_SOURCE_DIR}/beast/memory_pool.cpp
    #${PROJECT_SOURCE_DIR}/beast/dynamic_thread_pool.cpp
    #${PROJECT_SOURCE_DIR}/beast/listener.cpp
    #${PROJECT_SOURCE_DIR}/beast/libevent.cpp
    #${PROJECT_SOURCE_DIR}/beast/event_watcher.cpp
    #${PROJECT_SOURCE_DIR}/beast/event_loop.cpp
    #${PROJECT_SOURCE_DIR}/beast/connection.cpp
    #${PROJECT_SOURCE_DIR}/beast/event_timer.cpp
    #${PROJECT_SOURCE_DIR}/beast/event_loop_thread.cpp
    #${PROJECT_SOURCE_DIR}/beast/event_loop_pool.cpp
    #${PROJECT_SOURCE_DIR}/beast/tcp_server.cpp

    #${PROJECT_SOURCE_DIR}/example/example_for_tcp_server.cpp

)

add_executable(${PROJECT_NAME} ${BUTIL_SOURCES})
target_link_libraries(libatp /home/pengwang/cloud_platform/target/3rd/jsoncpp/libjsoncpp.a)
target_link_libraries(libatp /home/pengwang/cloud_platform/target/3rd/libevent/libevent_core.a)
target_link_libraries(libatp /home/pengwang/cloud_platform/target/3rd/gflags/libgflags.so)
target_link_libraries(libatp /home/pengwang/cloud_platform/target/3rd/glog/libglog.a)

