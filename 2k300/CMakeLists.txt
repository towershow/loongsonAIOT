cmake_minimum_required(VERSION 3.16)

# 设置交叉编译工具链
set(CMAKE_C_COMPILER "/opt/loongson-gnu-toolchain-8.3-x86_64-loongarch64-linux-gnu-rc1.4/bin/loongarch64-linux-gnu-gcc")
set(CMAKE_CXX_COMPILER "/opt/loongson-gnu-toolchain-8.3-x86_64-loongarch64-linux-gnu-rc1.4/bin/loongarch64-linux-gnu-g++")

# 项目名称
project(Camera5G)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)

# 检查main.cpp是否存在
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
    message(FATAL_ERROR "main.cpp not found in ${CMAKE_CURRENT_SOURCE_DIR}")
endif()

message(STATUS "Found main.cpp in ${CMAKE_CURRENT_SOURCE_DIR}")

# 系统头文件路径
include_directories(/opt/loongson-gnu-toolchain-8.3-x86_64-loongarch64-linux-gnu-rc1.4/loongarch64-linux-gnu/sysroot/usr/include/)

# 寻找OpenCV
set(OpenCV_FOUND FALSE)

# 方法1: 尝试find_package
find_package(OpenCV QUIET)
if(OpenCV_FOUND)
    message(STATUS "✓ Found OpenCV via find_package: ${OpenCV_VERSION}")
    include_directories(${OpenCV_INCLUDE_DIRS})
    set(OPENCV_LIBRARIES ${OpenCV_LIBS})
else()
    # 方法2: 手动查找
    message(STATUS "Trying manual OpenCV detection...")
    
    set(OPENCV_INCLUDE_PATHS
        "/home/yuqing/opencv/install/include"
        "/home/yuqing/opencv/install/include/opencv4"
        "/usr/local/include/opencv4"
        "/usr/include/opencv4"
    )
    
    set(OPENCV_LIB_PATHS
        "/home/yuqing/opencv/install/lib"
        "/usr/local/lib"
        "/usr/lib/loongarch64-linux-gnu"
    )
    
    # 查找头文件
    foreach(path ${OPENCV_INCLUDE_PATHS})
        if(EXISTS "${path}/opencv2/opencv.hpp")
            message(STATUS "✓ Found OpenCV headers at: ${path}")
            include_directories(${path})
            set(OpenCV_FOUND TRUE)
            break()
        endif()
    endforeach()
    
    # 查找库文件
    if(OpenCV_FOUND)
        foreach(path ${OPENCV_LIB_PATHS})
            if(EXISTS "${path}")
                link_directories(${path})
                message(STATUS "✓ Added OpenCV lib path: ${path}")
                break()
            endif()
        endforeach()
        
        set(OPENCV_LIBRARIES
            opencv_core
            opencv_imgproc
            opencv_imgcodecs
            opencv_videoio
            opencv_highgui
        )
    endif()
endif()

if(NOT OpenCV_FOUND)
    message(WARNING "OpenCV not found - building without camera support")
    add_definitions(-DNO_OPENCV=1)
    set(OPENCV_LIBRARIES "")
endif()

# 创建可执行文件 - 这是关键！
add_executable(camera_streaming main.cpp)

# 链接库
target_link_libraries(camera_streaming 
    ${OPENCV_LIBRARIES}
    pthread
)

# 添加编译选项
target_compile_options(camera_streaming PRIVATE
    -Wall
    -Wextra
    -O2
)

# 如果没有OpenCV，添加编译定义
if(NOT OpenCV_FOUND)
    target_compile_definitions(camera_streaming PRIVATE NO_OPENCV=1)
endif()

# 显示配置信息
message(STATUS "=== Build Configuration ===")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Target: camera_streaming") 
message(STATUS "Source: main.cpp")
message(STATUS "OpenCV: ${OpenCV_FOUND}")
if(OpenCV_FOUND)
    message(STATUS "OpenCV libs: ${OPENCV_LIBRARIES}")
endif()
message(STATUS "===========================")

# 设置可执行文件属性
set_target_properties(camera_streaming PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)