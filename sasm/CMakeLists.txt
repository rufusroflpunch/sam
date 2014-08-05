add_definitions("-std=c++11")

add_executable(sasm sasm.cpp)
add_executable(sasm-run sasm-run.cpp)

add_custom_target(sasm_full
  DEPENDS sasm sasm-run)
