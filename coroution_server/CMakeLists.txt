
# 设置文件名词
set (appName coroution_server)

# 指定 vcpkg 的工具链文件  
set(CMAKE_TOOLCHAIN_FILE "D:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")  

set(CMAKE_PREFIX_PATH "D:/vcpkg/installed/x64-windows/share")

# 包含库目录
include_directories ("./")


# 查找Boost库，添加所需的组件  
find_package(Boost REQUIRED COMPONENTS asio)
# protobuf所需库
find_package(absl CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)
# jsoncpp
find_package(jsoncpp CONFIG REQUIRED)

# 获取要编译的源代码
file (GLOB_RECURSE SC_FILES ./*.cpp)

# 编译可执行文件
add_executable (${appName} ${SC_FILES})

# 链接
target_link_libraries(${appName} PRIVATE
    protobuf::libprotobuf
    Boost::asio
    JsonCpp::JsonCpp
)

# 将Debug版本的DLL目录添加到可执行文件环境变量
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_command(TARGET ${appName} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            "D:/vcpkg/installed/x64-windows/debug/bin/libprotobufd.dll"
            $<TARGET_FILE_DIR:${appName}>
        COMMAND ${CMAKE_COMMAND} -E copy
            "D:/vcpkg/installed/x64-windows/debug/bin/abseil_dll.dll"
            $<TARGET_FILE_DIR:${appName}>
        COMMAND ${CMAKE_COMMAND} -E copy
            "D:/vcpkg/installed/x64-windows/debug/bin/jsoncpp.dll"
            $<TARGET_FILE_DIR:${appName}>
    )
endif()