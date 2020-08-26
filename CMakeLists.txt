cmake_minimum_required(VERSION 3.9.6)
project(libatp C CXX)

include(FindThreads)
include(FindProtobuf)

protobuf_generate_cpp(PROTO_SRC PROTO_HEADER rpc.proto echo_server.proto)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories(
    /home/pengwang/cloud_platform/framework/3rd
    /home/pengwang/cloud_platform/project/github/libatp/src
)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -pg -pipe -Wall -g -W")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fstrict-aliasing -Wno-invalid-offsetof -Wno-unused-parameter -fno-omit-frame-pointer -Wno-unused-function")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -luuid -lssl -lcrypto -lglog")

set(CMAKE_CXX_FLAGS "${CMAKE_CPP_FLAGS} -O2 -fsanitize=address -pipe -Wall -W -fPIC -fstrict-aliasing -Wno-invalid-offsetof -Wno-unused-parameter -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS "${CMAKE_CPP_FLAGS} -lpthread -luuid -lgflags -lcurl -lprotoc -lssl -lcrypto -lglog -ldl -lz")
set(CMAKE_C_FLAGS   "${CMAKE_CPP_FLAGS} -O2 -fsanitize=address -pipe -Wall -W -fPIC -fstrict-aliasing -Wno-unused-parameter -fno-omit-frame-pointer")

macro(use_cxx11)
if(CMAKE_VERSION VERSION_LESS "3.1.3")
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=++11")
    endif()
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    endif()
else()
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()
endmacro(use_cxx11)

use_cxx11()

set(BUTIL_SOURCES
    ${PROJECT_SOURCE_DIR}/src/net/atp_socket.cpp
    ${PROJECT_SOURCE_DIR}/src/net/atp_dynamic_thread_pool.cpp
    ${PROJECT_SOURCE_DIR}/src/net/atp_libevent.cpp
    ${PROJECT_SOURCE_DIR}/src/net/atp_channel.cpp
    ${PROJECT_SOURCE_DIR}/src/net/atp_tcp_conn.cpp
    ${PROJECT_SOURCE_DIR}/src/net/atp_listener.cpp
    #${PROJECT_SOURCE_DIR}/src/atp_memory_pool.cpp
    #${PROJECT_SOURCE_DIR}/src/atp_memory_policy.cpp
    ${PROJECT_SOURCE_DIR}/src/net/atp_event_watcher.cpp
    ${PROJECT_SOURCE_DIR}/src/net/atp_cycle_timer.cpp
    ${PROJECT_SOURCE_DIR}/src/net/atp_event_loop.cpp
    ${PROJECT_SOURCE_DIR}/src/net/atp_event_loop_thread_pool.cpp
    ${PROJECT_SOURCE_DIR}/src/net/atp_tcp_server.cpp
    #${PROJECT_SOURCE_DIR}/src/atp_rpc_channel.cpp
    #${PROJECT_SOURCE_DIR}/src/atp_rpc_server.cpp
    #${PROJECT_SOURCE_DIR}/src/atp_curl_engine.cpp

    ${PROJECT_SOURCE_DIR}/src/app/atp_base64.cpp
    ${PROJECT_SOURCE_DIR}/src/app/atp_curl_engine.cpp
    ${PROJECT_SOURCE_DIR}/src/app/atp_event_https.cpp
    # benchmark
    #${PROJECT_SOURCE_DIR}/examples/atp_buffer_benchmark.cpp
    #${PROJECT_SOURCE_DIR}/examples/atp_listener_benchmark.cpp
    ${PROJECT_SOURCE_DIR}/examples/atp_tcp_benchmark.cpp
    #${PROJECT_SOURCE_DIR}/examples/atp_tcp_multi_listener_benchmark.cpp
    #${PROJECT_SOURCE_DIR}/examples/atp_memory_pool_benchmark.cpp
    #${PROJECT_SOURCE_DIR}/examples/atp_rpc_server_benchmark.cpp
    #${PROJECT_SOURCE_DIR}/examples/atp_timing_wheel_benchmark.cpp
    #${PROJECT_SOURCE_DIR}/examples/atp_rpc_client.cpp
)

set(DYNAMIC_LIB
    ${GFLAGS_LIBRARY}
    ${PROTOBUF_LIBRARIES}
    ${SSL_LIB}
    ${CRYPTO_LIB}
    ${THRIFT_LIB}
    ${THRIFTNB_LIB}
    dl
)

add_executable(${PROJECT_NAME} ${BUTIL_SOURCES} ${PROTO_SRC})
target_link_libraries(libatp /home/pengwang/cloud_platform/target/3rd/jsoncpp/libjsoncpp.a)
target_link_libraries(libatp /home/pengwang/cloud_platform/target/3rd/libevent/libevent.a)
target_link_libraries(libatp /home/pengwang/cloud_platform/target/3rd/libevent/libevent_openssl.a)
target_link_libraries(libatp /home/pengwang/cloud_platform/target/3rd/gflags/libgflags.so)
target_link_libraries(libatp /home/pengwang/cloud_platform/target/3rd/glog/libglog.a)
target_link_libraries(libatp ${DYNAMIC_LIB})

INSTALL(FILES ${HEADERS} DESTINATION ${PROJECT_BINARY_DIR}/)
