cmake_minimum_required(VERSION 3.15.4)
project(
    jge
    VERSION 0.1.0
    DESCRIPTION "JGE: JohelEGP's Game Engine."
    HOMEPAGE_URL https://github.com/johelegp/jge
    LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake/)
include(JGEOptions)
include(JGEFindDependencies)

add_library(jge INTERFACE)
add_library(jge::jge ALIAS jge)
target_compile_features(jge INTERFACE cxx_std_20)
target_link_libraries(jge INTERFACE mp::units range-v3::range-v3 lift)
target_include_directories(
    jge INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                  $<INSTALL_INTERFACE:include>)

install(TARGETS jge lift EXPORT jge-targets)

export(EXPORT jge-targets NAMESPACE jge::)
file(COPY tools/cmake/jge-config.cmake DESTINATION .)
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    jge-config-version.cmake
    VERSION ${jge_VERSION}
    COMPATIBILITY SameMajorVersion)

install(
    EXPORT jge-targets
    DESTINATION lib/cmake/jge
    NAMESPACE jge::)
install(FILES tools/cmake/jge-config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/jge-config-version.cmake
        DESTINATION lib/cmake/jge)
install(DIRECTORY include/ DESTINATION include)

include(CTest)
if(BUILD_TESTING AND JGE_TEST)
    add_subdirectory(tests)
endif()

if(JGE_EXAMPLES)
    add_subdirectory(examples)
endif()
