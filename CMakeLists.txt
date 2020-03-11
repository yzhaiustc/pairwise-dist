cmake_minimum_required(VERSION 2.8)
link_directories(../mkldnn/lib/)
include_directories(mkldnn/include/)
if (APPLE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -march=native -Wall -liomp5 -lmklml")
endif()

if(UNIX AND NOT APPLE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -march=native -Wall -liomp5 -lmklml_gnu")
endif()

add_executable(mypdist pdist.c utils.c test.c)
#target_link_libraries(mypdist )