set(LIBRARY_TARGET_NAME ${PROJECT_NAME})

# Build the test binary
add_executable(mathopt_tester src/mathopt_tester.cc)
if(NOT "${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)") # If 64 bit windows build, don't add this
  add_executable(vec_range_tester src/vec_range_tester.cc)
endif()

target_include_directories(mathopt_tester PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
if(NOT "${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)") # If 64 bit windows build, don't add this
  target_include_directories(vec_range_tester PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()

# FIXME: Only on windows 64 builds there seems to errors becuase of ambiguous
# Polymorphism of the + operator in the DMScu_vectorRange header file. It might
# be becuase long and int being the same in Windows 64, but simply disabling one
# of the overloadeed operators didn't seemss to work. fix me!

# Enable ctest, testing so we can see if unit tests pass or fail in CI
enable_testing()
add_test(NAME mathopt_tester
         COMMAND mathopt_tester
         WORKING_DIRECTORY $<TARGET_FILE_DIR:mathopt_tester>)

if(NOT "${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)") # If 64 bit windows build, don't add this
  add_test(NAME vec_range_tester
          COMMAND vec_range_tester
          WORKING_DIRECTORY $<TARGET_FILE_DIR:vec_range_tester>)
endif()
