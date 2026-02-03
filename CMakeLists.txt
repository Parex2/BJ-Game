cmake_minimum_required(VERSION 3.28)

project(CardGame LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SQLite3_INCLUDE_DIR /usr/include)
set(SQLite3_LIBRARY /usr/lib/x86_64-linux-gnu/libsqlite3.so)
find_package(SQLite3 REQUIRED)



add_executable(CardGame 
	src/main.cpp
	src/Card.cpp
	src/Deck.cpp
	src/Player.cpp
	src/BlackJack.cpp
	src/BlackJackNew.cpp
)

target_link_libraries(CardGame PRIVATE SQLite::SQLite3)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(my_tests
    tests/card_test.cpp
)

target_link_libraries(my_tests
    PRIVATE
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(my_tests)
