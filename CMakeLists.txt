cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(CardGame)

add_executable(CardGame 
	src/main.cpp
	src/Card.cpp
	src/Deck.cpp
	src/Player.cpp
	src/BlackJack.cpp
	src/BlackJackNew.cpp
)

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
