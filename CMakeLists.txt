cmake_minimum_required(VERSION 3.24)
project(jocul_patratelor_glisante)

set(CMAKE_CXX_STANDARD 14)

add_executable(jocul_patratelor_glisante main.cpp)

target_include_directories(jocul_patratelor_glisante PRIVATE "C:/winbgim")
target_link_directories(jocul_patratelor_glisante PRIVATE "C:/winbgim")
target_link_libraries(jocul_patratelor_glisante bgi gdi32 comdlg32 uuid oleaut32 ole32)
