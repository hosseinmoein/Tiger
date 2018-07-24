set(LIBRARY_TARGET_NAME ${PROJECT_NAME})

target_sources(${LIBRARY_TARGET_NAME} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/DMScu_MathOperators.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/DMScu_StepVectorRange.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/DMScu_VectorRange.h>
  )

# Build the test binary
add_executable(mathopt_tester src/mathopt_tester.cc)
add_executable(vec_range_tester src/vec_range_tester.cc)
target_include_directories(mathopt_tester PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(vec_range_tester PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Enable ctest, testing so we can see if unit tests pass or fail in CI
enable_testing()
add_test(NAME mathopt_tester
         COMMAND mathopt_tester
         WORKING_DIRECTORY $<TARGET_FILE_DIR:mathopt_tester>)
add_test(NAME vec_range_tester
         COMMAND vec_range_tester
         WORKING_DIRECTORY $<TARGET_FILE_DIR:vec_range_tester>)
