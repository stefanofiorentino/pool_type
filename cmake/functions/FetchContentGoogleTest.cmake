function(fetch_content_googletest)
    set(OLD_BUILD_TESTING ${BUILD_TESTING})
    set(BUILD_TESTING OFF CACHE INTERNAL "")  # Forces the value

    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG master
            GIT_SHALLOW 1
    )

    FetchContent_GetProperties(googletest)

    if(NOT googletest_POPULATED)
        FetchContent_Populate(googletest)
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()

    add_library(GTest::Main ALIAS gtest_main)

    target_compile_features(gtest PUBLIC cxx_std_17)
    target_compile_features(gtest_main PUBLIC cxx_std_17)
    target_compile_features(gmock PUBLIC cxx_std_17)
    target_compile_features(gmock_main PUBLIC cxx_std_17)

    set(BUILD_TESTING ${OLD_BUILD_TESTING} CACHE INTERNAL "")
endfunction()