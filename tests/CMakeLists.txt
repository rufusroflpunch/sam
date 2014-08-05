add_executable(test test.cpp)

add_custom_target(run_tests
  COMMAND test -c)
