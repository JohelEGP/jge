find_package(SFML 2.5 REQUIRED COMPONENTS graphics)

add_executable(lpc main.cpp)
target_link_libraries(lpc PRIVATE jge::jge sfml-graphics)
