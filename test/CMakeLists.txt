set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

function(ADD_THIS_TEST TEST_NAME TEST_SOURCE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})

    target_link_libraries(
            ${TEST_NAME}
            PRIVATE
            GTest::Main
            gmock
            Threads::Threads
    )

    target_include_directories(${TEST_NAME} PRIVATE ${PROJECT_SOURCE_DIR})
    add_test(NAME test_${TEST_NAME} COMMAND $<TARGET_FILE:${TEST_NAME}>)
endfunction()

ADD_THIS_TEST(emitter emitter.cpp)
ADD_THIS_TEST(finite_state_machine finite_state_machine.cpp)
