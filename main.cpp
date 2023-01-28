#include <iostream>
#include "fmt/core.h"
#include <opencv4/opencv2/opencv.hpp>
#include<opencv2/core.hpp> /*包含库的核心功能*/
#include<opencv2/imgproc.hpp> /*包括主要的图像处理函数*/
#include<opencv2/highgui.hpp> /*提供了读写图像和视频的函数以及一些用户交互函数*/

int execute_command(const std::string &command, std::string *output = nullptr,
                    bool redirect_stderr = false) {
    const auto &cmd = redirect_stderr ? command + " 2>&1" : command;
    auto pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        //记录日志
        return -1;
    }
    {
        char buffer[1024] = {0};
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            if (output) {
                output->append(buffer);
            }
//            printf("%s",buffer);
        }
    }
    return pclose(pipe);
}

using namespace cv;
Mat image_show(const char*image_path)
{
    //从文件中读入图像
    Mat img = imread(image_path, 1);

    //如果读入图像失败
    if (img.empty())
    {
        std::cerr << "Can not load image:" << image_path << std::endl;
        //fprintf(stderr, "Can not load image %s\n", image_path);
        abort();//异常退出
    }
    //显示图像
//    imshow("image", img);
    std::cout << "image:" << img.rows << "," << img.cols << std::endl;
    //此函数等待按键，按键盘任意键就返回
//    waitKey(0);
    /*官方解释是highgui
    *没有给imshow绘制处理的时间。
    *需要在imshow添加waitKey()
    *waitKey()单位是ms*/
    return img;
}


int main() {
    printf("%s\n", CV_VERSION);
    image_show("./1.jpeg");
    std::string shell_command = {" /etc/passwd"};

    std::string output;
    if ((execute_command(fmt::format("cat {0} 2>/dev/null", shell_command), &output)) == 0) {
        printf("%s\n", output.c_str());
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
