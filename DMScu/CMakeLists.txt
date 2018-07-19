# set(LIBRARY_TARGET_NAME Matrix)

# List of header files.
set(${LIBRARY_TARGET_NAME}_HDR
  include/DMScu_MathOperators.h
  include/DMScu_StepVectorRange.h
  include/DMScu_VectorRange.h
)

# # Specify include directories for both compilation and installation process.
# # The $<INSTALL_PREFIX> generator expression is useful to ensure to create 
# # relocatable configuration files, see https://cmake.org/cmake/help/latest/manual/cmake-packages.7.html#creating-relocatable-packages
# target_include_directories(${LIBRARY_TARGET_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
# 							 "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

# # Specify installation targets, typology and destination folders.
# install(TARGETS ${LIBRARY_TARGET_NAME}
#         EXPORT  ${PROJECT_NAME}
#         LIBRARY       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT shlib
#         ARCHIVE       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT lib
#         RUNTIME       DESTINATION "${CMAKE_INSTALL_BINDIR}"                            COMPONENT bin
#         PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${LIBRARY_TARGET_NAME}" COMPONENT dev)

# # Create and install CMake configuration files for your project that are
# # necessary to for other projects to call find_package().
# #
# # Note that it is extremely important to use exactly the project name while installing configuration
# # files (you can use PROJECT_NAME variable to avoid any possible error). This is required to allow
# # find_package() to properly look for the installed library in system path, in particular in Windows
# # when the installation is performed in the default path.
# #
# # install_basic_package_files() comes with many input parameters to customize the configuration
# # files. The parameters used in the following call provide basic versions of CMake configuration
# # files.  See install_basic_package_files() documentation found in ./cmake folder.
# #
# # Note that if your library depends from other libraries, you are probably
# # required to used the install_basic_package_files() DEPENDENCIES option.
# include(InstallBasicPackageFiles)
# install_basic_package_files(${PROJECT_NAME}
#                             VERSION ${${PROJECT_NAME}_VERSION}
#                             DEPENDENCIES "DMScu REQUIRED"
#                             COMPATIBILITY AnyNewerVersion
#                             EXPORT ${PROJECT_NAME}
#                             VARS_PREFIX ${PROJECT_NAME}
#                             NO_CHECK_REQUIRED_COMPONENTS_MACRO)

# # Add the uninstall target
# include(AddUninstallTarget)

# Build the test binary
add_executable(mathopt_tester src/mathopt_tester.cc)
add_executable(vec_range_tester src/vec_range_tester.cc)

# Enable ctest, testing so we can see if unit tests pass or fail in CI
enable_testing()
add_test(NAME mathopt_tester
         COMMAND mathopt_tester
         WORKING_DIRECTORY $<TARGET_FILE_DIR:mathopt_tester>)
add_test(NAME vec_range_testre
         COMMAND vec_range_testre
         WORKING_DIRECTORY $<TARGET_FILE_DIR:vec_range_testre>)
