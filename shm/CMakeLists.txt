cmake_minimum_required(VERSION 3.16)
project(shm)

link_libraries(rt)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/shm_w SHM_W_SRCS)
add_executable(shm_w ${SHM_W_SRCS})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/shm_r SHM_R_SRCS)
add_executable(shm_r ${SHM_R_SRCS})

