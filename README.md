##conan c++包管理器
###说明
    主页 https://conan.io/
    库搜索 https://conan.io/center/
###环境设置
    安装conan包管理器    
    pip install conan
    增加shell环境变量
    因为默认 conan是安装到用户的 .local/bin下的，不存在PATH内
    sudo gedit sudo gedit /etc/bash.bashrc
    在export PATH=$PATH: 可能有很多路径添加${HOME}/.local/bin
    conan的文件都是用户的.conan文件夹下
    新建默认配置
    conan profile new default
    conan profile update settings.compiler.libcxx=libstdc++11 default
    新建arm64配置
    conan profile new arm64
    具体内容
        target_host=aarch64-linux-gnu
        standalone_toolchain=/usr/local/arm64/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu
        cc_compiler=gcc
        cxx_compiler=g++
        [settings]
        arch=armv8
        os=Linux
        build_type=Release
        compiler=gcc
        compiler.libcxx=libstdc++11
        compiler.version=7.3
        [options]
        [build_requires]
        [env]
        PATH=[$standalone_toolchain/bin]
        CHOST=$target_host
        AR=$target_host-ar
        AS=$target_host-as
        RANLIB=$target_host-ranlib
        LD=$target_host-ld
        STRIP=$target_host-strip
        CC=$target_host-$cc_compiler
        CXX=$target_host-$cxx_compiler

        这里重点是standalone_toolchain的路径一定要和实际相符

###工程设置
    ide 选用clion 这里clion就要在File | Settings | Build, Execution, Deployment | Toolchains 新添加arm64的交叉链地址 
    同时在编译arm64目标的时候 在File | Settings | Build, Execution, Deployment | CMake 的Toolchain项选择对应的链
    在工程目录下新建 conan文件夹
        在conan文件夹下写入配置文件 conanfile.txt,必须是这个名字，文件内容如下
        [requires]
        fmt/9.1.0
        [generators]
        cmake

        在该目录下新建build 进入build输入命令拉取相应的库并编译 -pr 是指定profile文件 --build 是指定库
        rm -rf * 清除上次的配置文件
        conan install .. -pr arm64 --build=fmt
        
        安装好的库可以通过 conan search fmt/9.1.0@查看具体情况
        
    剩下的就是clion上的编译了 编译的时候注意 File | Settings | Build, Execution, Deployment | CMake Profiles勾选相应的 enable profile
### opencv库 aarch64
    修改ffmpeg内的export/conanfile.py
    546行增加
    if tools.get_env("STRIP"):
        args.append("--strip={}".format(tools.get_env("STRIP")))
    主要是为了生成ffmpeg执行文件的时候，修改strip值

    修改libx264内的export/conanfile.py
        26增加
            "disable_asm": [True, False],
        32增加
            "disable_asm": True,
        102增加
            if self.options.disable_asm:
                args.append("--disable-asm")
        原因交叉编译链的as工具编译.S文件不支持x86的asm
    
    conanfile.txt
        [requires]
        fmt/9.1.0
        opencv/4.5.5
        [options]
        opencv:shared=True
        opencv:with_gtk=False
        opencv:with_tiff=False
        vulkan-loader:with_wsi_xlib=False
        pulseaudio:with_x11=False
        libx264:disable_asm=True
        ffmpeg:with_asm=False
        ffmpeg:with_vulkan=False
        ffmpeg:with_xcb=False
        ffmpeg:with_vaapi=False
        ffmpeg:with_vdpau=False
        [generators]
        cmake