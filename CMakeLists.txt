cmake_minimum_required(VERSION 3.19)
project(conanTest)

set(CMAKE_CXX_STANDARD 11)
# 引入Conan的CMake
set(CONAN_DISABLE_CHECK_COMPILER 1)
if (${ARCH} STREQUAL "x64")
    execute_process(COMMAND sh build_local.sh
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/conan)
else ()
    execute_process(COMMAND sh build_aarch64.sh
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/conan)
endif ()
include(conan/build/conanbuildinfo.cmake)
set(CMAKE_C_FLAGS "-Wl,-rpath=./:./lib")
set(CMAKE_CXX_FLAGS "-Wl,-rpath=./:./lib")
conan_basic_setup(KEEP_RPATHS)
file(GLOB allCopyFiles  "conan/build/bin/*")
file(COPY ${allCopyFiles} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lib FOLLOW_SYMLINK_CHAIN)
link_directories(${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lib)


#重新定义当前目标的源文件的__FILE__宏
function(redefine_file_macro targetname)
    #获取当前目标的所有源文件
    get_target_property(source_files "${targetname}" SOURCES)
    #遍历源文件
    foreach (sourcefile ${source_files})
        #获取当前源文件的编译参数
        get_property(defs SOURCE "${sourcefile}"
                PROPERTY COMPILE_DEFINITIONS)
        #获取当前文件的绝对路径
        get_filename_component(filepath "${sourcefile}" ABSOLUTE)
        #将绝对路径中的项目路径替换成空,得到源文件相对于项目路径的相对路径
        string(REPLACE ${PROJECT_SOURCE_DIR}/ "" relpath ${filepath})
        #将我们要加的编译参数(__FILE__定义)添加到原来的编译参数里面
        list(APPEND defs "__FILE__=\"${relpath}\"")
        #重新设置源文件的编译参数
        set_property(
                SOURCE "${sourcefile}"
                PROPERTY COMPILE_DEFINITIONS ${defs}
        )
    endforeach ()
endfunction()

add_definitions(-D${ARCH})
add_executable(conanTest main.cpp )
target_link_libraries(conanTest ${CONAN_LIBS})
