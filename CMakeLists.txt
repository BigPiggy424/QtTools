cmake_minimum_required(VERSION 3.25)

# 设置项目名和版本号
project(QtTools VERSION 1.0 LANGUAGES CXX)

# 指定c++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 设置Qt5路径
set(Qt5_DIR "{Qt路径}/lib/cmake/Qt5")
# 比如set(Qt5_DIR "D:/Qt/Qt5.14.2/5.14.2/msvc2017_64/lib/cmake/Qt5")

# 若需要跳过xxx.cpp的moc处理
# set_source_files_properties(xxx.cpp PROPERTIES SKIP_AUTOMOC True)

# 搜索包名，并指定项目需要那些组件（多个组件用空格隔开）
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# 添加工程头文件的搜索路径（多个路径空格分开）
include_directories(include)

# 递归地找出所有.h和.hpp后缀的文件
file(GLOB_RECURSE HEADERS "*.h" "*.hpp")
file(GLOB_RECURSE RESOURCES "*.qrc")

# 将src文件夹下的源文件添加到变量SOURCES中
aux_source_directory(src SOURCES)

add_library(${PROJECT_NAME} STATIC
            ${SOURCES}
            ${HEADERS}
            ${RESOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}
        LIBRARY DESTINATION ${LIBRARY_OUTPUT_PATH})
