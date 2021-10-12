cmake_minimum_required(VERSION 3.3.2)

set(PROJECT_NAME "fpga_vdf_discriminant")
project(${PROJECT_NAME} LANGUAGES CXX C)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(XILINX_PATH "/home/mjhatamy/Applications/Xilinx/Vitis_HLS/2021.1")

set(CMAKE_MODULE_PATH
        ${CMAKE_CURRENT_LIST_DIR}/cmake
        ${CMAKE_MODULE_PATH}
        )

find_package(GMP REQUIRED)
#find_package(GMPXX REQUIRED)
#find_package(FLINT REQUIRED)
#find_package(MPFR REQUIRED)

include_directories(
        ${INCLUDE_DIRECTORIES}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${XILINX_PATH}/include
        ${XILINX_PATH}/include/app_sync
        ${XILINX_PATH}/include/etc
)

add_executable(${PROJECT_NAME}
        fpga_vdf_discriminant-tb.cpp
        fpga_vdf_discriminant.h
        fpga_vdf_discriminant.cpp
        VDFTypes.h
        VDFTypes.cpp
        Utils.h
        Utils.cpp

        ${GMP_INCLUDE_DIR}
        ${GMPXX_INCLUDE_DIR}

        )

target_link_libraries(${PROJECT_NAME}
        ${GMP_LIBRARIES}
        ${GMPXX_LIBRARIES}
        )

