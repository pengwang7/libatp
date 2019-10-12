cmake_minimum_required(VERSION 3.9.6)
project(libatp)



include_directories(
    /home/pengwang/cloud_platform/framework/3rd
    /home/pengwang/cloud_platform/project/libatp/src
)

#find_package(GFLAGS REQUIRED)

#set(GFLAGS_INCLUDE_PATH "/home/wangpeng/WorkSpaceForMe/PlatformForMe/Framework/3rd")

#execute_process(
#    COMMAND bash -c "grep \"namespace [_A-Za-z0-9]\\+ {\" ${GFLAGS_INCLUDE_PATH}/gflags/gflags_declare.h | head -1 | awk '{print $2}' | tr -d '\n'"
#    OUTPUT_VARIABLE GFLAGS_NS
#)


#if(${GFLAGS_NS} STREQUAL "GFLAGS_NAMESPACE")
#    execute_process(
#        COMMAND bash -c "grep \"#define GFLAGS_NAMESPACE [_A-Za-z0-9]\\+\" ${GFLAGS_INCLUDE_PATH}/gflags/gflags_declare.h | head -1 | awk '{print $3}' | tr -d '\n'"
#        OUTPUT_VARIABLE GFLAGS_NS
#    )
#endif()

#set(CMAKE_CXX_FLAGS "${DEFINE_CLOCK_GETTIME} -DGFLAGS_NS=${GFLAGS_NS}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0 -pg -pipe -Wall -g -W")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fstrict-aliasing -Wno-invalid-offsetof -Wno-unused-parameter -fno-omit-frame-pointer -Wno-unused-function")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -lglog")

set(BUTIL_SOURCES
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
target_link_libraries(libatp /home/pengwang/cloud_platform/target/3rd/libevent/libevent_core.a)
target_link_libraries(libatp /home/pengwang/cloud_platform/target/3rd/gflags/libgflags.so)
target_link_libraries(libatp /home/pengwang/cloud_platform/target/3rd/glog/libglog.a)


#set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

