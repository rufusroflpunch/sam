project(Tutorial)
set(CMAKE_CXX_FLAGS "-std=c++11")
add_executable(loop loop.cpp)
add_executable(hello hello.cpp)
add_executable(name name.cpp)


add_library(vm ../vm.cpp)
target_link_libraries(loop vm)
target_link_libraries(hello vm)
target_link_libraries(name vm)
