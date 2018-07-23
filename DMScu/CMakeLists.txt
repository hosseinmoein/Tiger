# List of header files.
set(${LIBRARY_TARGET_NAME}_HDR
  include/DMScu_MathOperators.h
  include/DMScu_StepVectorRange.h
  include/DMScu_VectorRange.h
)

# Build the test binary
add_executable(mathopt_tester src/mathopt_tester.cc)
add_executable(vec_range_tester src/vec_range_tester.cc)

# Enable ctest, testing so we can see if unit tests pass or fail in CI
enable_testing()
add_test(NAME mathopt_tester
         COMMAND mathopt_tester
         WORKING_DIRECTORY $<TARGET_FILE_DIR:mathopt_tester>)
add_test(NAME vec_range_tester
         COMMAND vec_range_tester
         WORKING_DIRECTORY $<TARGET_FILE_DIR:vec_range_tester>)
