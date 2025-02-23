# CMakeList.txt: boost_test 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

# 指定 vcpkg 的工具链文件  
set(CMAKE_TOOLCHAIN_FILE "D:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")  

set(CMAKE_PREFIX_PATH "D:/vcpkg/installed/x64-windows/share")

# 查找Boost库，添加所需的组件  
find_package(Boost REQUIRED COMPONENTS asio)
# protobuf所需库
find_package(absl CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)
# jsoncpp
find_package(jsoncpp CONFIG REQUIRED)

# 将源代码添加到此项目的可执行文件。  
add_executable (boost_test "boost_test.cpp" "msg.pb.cc")  

if (CMAKE_VERSION VERSION_GREATER 3.12)  
  set_property(TARGET boost_test PROPERTY CXX_STANDARD 20)  
endif()  

# 将库链接到可执行文件  
target_link_libraries(boost_test PRIVATE
    protobuf::libprotobuf
    Boost::asio
    JsonCpp::JsonCpp
)

# 将Debug版本的DLL目录添加到可执行文件环境变量
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_command(TARGET boost_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            "D:/vcpkg/installed/x64-windows/debug/bin/libprotobufd.dll"
            $<TARGET_FILE_DIR:boost_test>
        COMMAND ${CMAKE_COMMAND} -E copy
            "D:/vcpkg/installed/x64-windows/debug/bin/abseil_dll.dll"
            $<TARGET_FILE_DIR:boost_test>
        COMMAND ${CMAKE_COMMAND} -E copy
            "D:/vcpkg/installed/x64-windows/debug/bin/jsoncpp.dll"
            $<TARGET_FILE_DIR:boost_test>
    )
endif()