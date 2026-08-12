# CMakeList.txt: SmartBoard 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.16)

project ("SmartBoard")

# 设置Qt的查找路径
set(CMAKE_PREFIX_PATH "C:/Qt/6.11.1/mingw_64")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# 将源代码添加到此项目的可执行文件。
add_executable (SmartBoard  
        scr/cpp/Background.cpp
        scr/h/Background.h
        scr/h/GestureBrush.h
        scr/cpp/GestureBrush.cpp
        scr/cpp/Main.cpp
        scr/h/Board.h
        scr/cpp/Board.cpp
        scr/h/ToolButtons.h
        scr/cpp/ToolButtons.cpp
        scr/cpp/SingleToolBtn.cpp
        scr/h/SingleToolBtn.h
        scr/h/BtnTypes.h
)


# 4. 设置 EXE 输出目录（关键！）
set_target_properties(SmartBoard PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

# 设置 EXE 文件名称
set_target_properties(SmartBoard PROPERTIES
    OUTPUT_NAME "SmartBoard$<$<CONFIG:Debug>:_d>"
)

target_link_libraries(SmartBoard PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)
