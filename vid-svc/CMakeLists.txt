cmake_minimum_required(VERSION 3.24)
project(vid_svc)

set(CMAKE_CXX_STANDARD 17)

include_directories(../common)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS log log_setup program_options date_time REQUIRED)

add_executable(vid_svc main.cpp VideoSvcSim.cpp VideoSvcSim.h ../common/AmqpPub.cpp ../common/AmqpSub.cpp ../common/CommonDefs.h ../common/Utils.cpp)

target_precompile_headers(vid_svc
        PRIVATE
        <boost/log/trivial.hpp>
        )

target_link_libraries(vid_svc
        ${Boost_LIBRARIES}
        SimpleAmqpClient
        rabbitmq
        pthread
        )
