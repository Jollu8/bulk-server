cmake_minimum_required(VERSION 3.23)

# PROJECT
set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})
set(PROJECT_NAME "bulk_server" CACHE STRING "Project name")
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})

find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(Threads)
include_directories(${Boost_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# sources
message("My_Message Done source file from /lib")
file(GLOB SOURCES main.cpp ${PROJECT_SOURCE_DIR}/lib/*.h ${PROJECT_SOURCE_DIR}/lib/*.cpp)
# headers

message("My_Message Done headers file from /lib")


add_executable(${PROJECT_NAME}  ${SOURCES} )
add_executable(bulk_client client.cpp)
message("My_Message created bulk_client and bulk_server")
target_link_libraries(
        bulk_server
        ${Boost_LIBRARIES}
        ${Boost_THREAD_LIBS_INIT}
)
target_compile_options(bulk_server PRIVATE
        -Wall -Wextra -pedantic -Werror
        )
target_link_libraries(
        bulk_client
        ${Boost_LIBRARIES}
        ${Boost_THREAD_LIBS_INIT}
)
target_compile_options(bulk_client PRIVATE
        -Wall -Wextra -pedantic -Werror
        )

# package
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)



