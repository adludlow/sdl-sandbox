cmake_minimum_required(VERSION 2.6)
project(sdlSandbox)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${sdlSandbox_SOURCE_DIR}/cmake")

set(BIN_DIR ${sdlSandbox}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")

include_directories(/usr/include/SDL2 /usr/include/boost )

add_executable(sdlSandbox src/main.cpp src/transform.cpp src/MovingRenderablePolygon.cpp )
if(MINGW)
	target_link_libraries(sdlSandbox /usr/local/lib/libSDL2.dll.a )
else()
	target_link_libraries(sdlSandbox /usr/lib/libSDL2.so /usr/lib/libSDL2_image.so)
endif()

install(TARGETS sdlSandbox RUNTIME DESTINATION ${BIN_DIR})
file(COPY images DESTINATION .)
