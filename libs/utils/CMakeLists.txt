project(utils C CXX)

set(SOURCES
    src/FunctionWrapper.cpp
    src/ThreadPool.cpp
    src/ThreadPoolSingleton.cpp
)

set(HEADERS
    include/yabil/utils/FunctionWrapper.h
    include/yabil/utils/IterUtils.h
    include/yabil/utils/ThreadPool.h
    include/yabil/utils/ThreadPoolSingleton.h
    include/yabil/utils/TypeUtils.h
)

set(TESTS
    test/FunctionWrapper_tests.cpp
    test/IterUtils_tests.cpp
    test/ThreadPool_tests.cpp
    test/ThreadPoolSingleton_tests.cpp
)

add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE ${SOURCES}
    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES ${HEADERS}
)

set_common_properties(${PROJECT_NAME})
add_test_target(${PROJECT_NAME} ${TESTS})
