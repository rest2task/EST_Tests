# cmake 最低版本要求为3.0
cmake_minimum_required(VERSION 3.0)
set(CMAKE_C_STANDARD 99)
# 工程名叫HmiApp
project(HmiApp)

# 定义了一个EMWIN选项 是否使用emwin库 默认不使用
option(EMWIN "use emwin library")

# 定义表示使用M788面板
add_definitions(-DM788_PANEL)

# 定义表示使用PANEL-TC面板
add_definitions(-DMPANEL_TC)

# 定义表示没有带数字按键板的面板
#add_definitions(-DPANEL_NOKEYBOARD)

# 定义表示使用A40i的面板
add_definitions(-DA40I)

# 定义表示使用背光LVDS的面板
add_definitions(-DBRIGHT_LVDS)

# find module
# 通过ENV前缀来访问环境变量CMAKE_PLATFORM_PATH D:\ESTOHMI\TSAPP\CMakeEnv
set(PLATFORM_TOP_PATH $ENV{CMAKE_PLATFORM_PATH})

# 环境变量不存在则定义PLATFORM_TOP_PATH为定义了顶级CMakeLists.txt的根目录 在这里是apppro的路径
if("${PLATFORM_TOP_PATH}" STREQUAL "")
    set(PLATFORM_TOP_PATH ${CMAKE_SOURCE_DIR})
endif()

# 打印变量PLATFORM_TOP_PATH的值
message("PLATFORM_TOP_PATH: ${PLATFORM_TOP_PATH}")

# 在${CMAKE_MODULE_PATH}中添加包含Find<Package>.cmake目录${PLATFORM_TOP_PATH}下的cmake文件夹路径
set(CMAKE_MODULE_PATH ${PLATFORM_TOP_PATH}/cmake)

# 链接外部库 查找Findplatform.cmake 和 Findqtdeploy.cmake 找不到则立即停止cmake
find_package(platform REQUIRED)
find_package(qtdeploy REQUIRED)

# source files
# 把app mod privmod pages下所有源文件存储在变量HMI_APP_SRC中
aux_source_directory(app HMI_APP_SRC)
aux_source_directory(mod HMI_APP_SRC)
aux_source_directory(privmod HMI_APP_SRC)
aux_source_directory(pages HMI_APP_SRC)
aux_source_directory(opcua HMI_APP_SRC)

# target
# 定义缓存变量CMAKE_PANELUI_PATH STRING表示文本行 字符串"panel ui project path"则是描述信息
set(CMAKE_PANELUI_PATH "${CMAKE_CURRENT_LIST_DIR}/../panelUI" CACHE STRING "panel ui project path")
# 设置运行时可执行文件应构建到的目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_PANELUI_PATH}/Release)
# 定义变量TARGET_NAME PROJECT_NAME:项目名称 PLATFORM_NAME:平台名称
set(TARGET_NAME ${PROJECT_NAME}_${PLATFORM_NAME})
# 定义缓存变量HMI_APP_NAME INTERNAL表示文本行,只用于内部 字符串"HmiApp name"则是描述信息
set(HMI_APP_NAME ${TARGET_NAME} CACHE INTERNAL "HmiApp name")
# 最终生成的可执行文件名叫${TARGET_NAME} 使用源文件${HMI_APP_SRC} 添加${PLATFORM_INCLUDES}在文件列表里面显示头文件
add_executable(${TARGET_NAME} ${HMI_APP_SRC} ${PLATFORM_INCLUDES})

# include directories
# 指定编译目标文件的时候需要包含的路径或内容
target_include_directories(${TARGET_NAME} PRIVATE
    ${PLATFORM_INCLUDE_DIR}
    app
    mod
    opcua
    pages
    privmod
)

# link
# 把目标文件与库文件进行链接
target_link_libraries(${TARGET_NAME} ${PLATFORM_LIBRARY})

# plugins
# 定义变量PLUGIN_PATH-文件名替代表达式
set(PLUGIN_PATH "${CMAKE_CURRENT_LIST_DIR}/plugin/${PLATFORM_NAME}/*.a")
# 产生一个匹配${PLUGIN_PATH}的文件列表并将它存储到变量PLUGIN_LIBS中
file(GLOB PLUGIN_LIBS ${PLUGIN_PATH})
# 循环列表打印plugin文件下的库文件
foreach(PLUGIN_LIB IN LISTS PLUGIN_LIBS)
    message("find plugin: " ${PLUGIN_LIB})
endforeach()
# 把目标文件与库文件进行链接
target_link_libraries(${TARGET_NAME} ${PLUGIN_LIBS})

# extra process
# 自定义函数platform_extra_process
platform_extra_process(${TARGET_NAME})

# qtcreator auto deploy
# 自定义函数qtdeploy_init qtdeploy_add_file
qtdeploy_init("/")
qtdeploy_add_file(${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET_NAME} "opt")
