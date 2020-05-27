include(FetchContent)
FetchContent_Declare(
    CPM
    GIT_REPOSITORY https://github.com/TheLartians/CPM.cmake
    GIT_TAG master
    GIT_SHALLOW True)
FetchContent_MakeAvailable(CPM)
include(${cpm_SOURCE_DIR}/cmake/CPM.cmake)

CPMFindPackage(
    NAME lift
    GITHUB_REPOSITORY rollbear/lift
    GIT_TAG master
    GIT_SHALLOW True)

CPMFindPackage(
    NAME fmt
    GITHUB_REPOSITORY fmtlib/fmt
    GIT_TAG master
    GIT_SHALLOW True)

CPMFindPackage(
    NAME range-v3
    GITHUB_REPOSITORY ericniebler/range-v3
    GIT_TAG master
    GIT_SHALLOW True
    DOWNLOAD_ONLY True)
if(range-v3_ADDED)
    add_library(range-v3::range-v3 INTERFACE IMPORTED GLOBAL)
    target_include_directories(range-v3::range-v3
                               INTERFACE ${range-v3_SOURCE_DIR}/include)
endif()

CPMFindPackage(
    NAME mp-units
    GITHUB_REPOSITORY mpusz/units
    GIT_TAG master
    GIT_SHALLOW True
    DOWNLOAD_ONLY True)
if(mp-units_ADDED)
    add_subdirectory(${mp-units_SOURCE_DIR}/src ${mp-units_BINARY_DIR})
endif()

CPMFindPackage(
    NAME jegp_cmake_modules
    GITHUB_REPOSITORY johelegp/jegp.cmake_modules
    GIT_TAG master
    GIT_SHALLOW True)
list(APPEND CMAKE_MODULE_PATH ${jegp_cmake_modules_SOURCE_DIR})
