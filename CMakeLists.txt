# CMake entry point
cmake_minimum_required (VERSION 3.1)
project (mhxy)

set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/install)
add_definitions(
	-std=c++11
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

link_directories(/Users/oceancx/CommonLib/lib/)

include_directories(
	/Users/oceancx/CommonLib/include/
	include/
)

find_library(OpenGL_LIBRARY OpenGL)
find_library(Cocoa_LIBRARY Cocoa)
find_library(IOKit_LIBRARY IOKit)
find_library(CoreVideo_LIBRARY CoreVideo)

set(ALL_LIBS
	${Cocoa_LIBRARY} 
	${OpenGL_LIBRARY} 
	${IOKit_LIBRARY} 
	${CoreVideo_LIBRARY}
	glfw3
	GLEW_1130
	SOIL
)


ADD_SUBDIRECTORY(src bin)

INSTALL(FILES COPYRIGHT README DESTINATION ${CMAKE_INSTALL_PREFIX})