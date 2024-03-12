cmake_minimum_required(VERSION 3.1)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(cuda_fractals LANGUAGES CUDA CXX)

add_executable(cuda_fractals main.cpp cuda_kernel.cu cuda_kernel.cuh)
set_property(TARGET cuda_fractals PROPERTY CXX_STANDARD 17)


set(MAIN_DIR ${CMAKE_CURRENT_SOURCE_DIR})
#add_subdirectory(CUDA)
#add_subdirectory(OpenGL)
#add_subdirectory(ImGui)


target_include_directories(cuda_fractals 
	PUBLIC ${MAIN_DIR})
#	PUBLIC ${MAIN_DIR}/CUDA
#	PUBLIC ${MAIN_DIR}/OpenGL
#	PUBLIC ${MAIN_DIR}/ImGui)

# do not generate ZERO_CHECK
set(CMAKE_SUPPRESS_REGENERATION true)