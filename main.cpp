#include <iostream>
#include "fmt/core.h"

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

int main() {
    std::string shell_command = {" /etc/passwd"};

    std::string output;
    if ((execute_command(fmt::format("cat {0} 2>/dev/null", shell_command),&output)) == 0) {
        printf("%s\n", output.c_str());
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
