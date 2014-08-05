set(CMAKE_CXX_FLAGS "-std=c++11")
add_executable(loop loop.cpp)
add_executable(hello hello.cpp)
add_executable(name name.cpp)

add_custom_target(samples
  DEPENDS loop hello name)
