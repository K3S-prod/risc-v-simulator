#pragma once

#include <iostream>

namespace sim {

class Controller {
public:
    enum class Command {
        INVALID,
        NEXT,
        NEXT_SEVERAL,
        CONTINUE,
        QUIT,
        REG_PRINT,
        SET_BREAKPOINT,
    };

    void RequireCommand() {
        std::cout << "(rvsim) ";
        char c = 0;
        std::cin >> c;
        switch (c)
        {
            case 'n':
                cmd_ = Command::NEXT;
                break;
            
            case 'N':
                cmd_ = Command::NEXT_SEVERAL;
                std::cin >> num_;
                if (std::cin.peek() != '\n') {
                    std::string str;
                    std::cin >> str;
                    std::cout << "Unexpected: '" << str << "'" <<std::endl;
                    cmd_ = Command::INVALID;
                }
                break;
            
            case 'c':
                cmd_ = Command::CONTINUE;
                break;

            case 'q':
                cmd_ = Command::QUIT;
                break;

            case 'r':
                cmd_ = Command::REG_PRINT;
                std::cin >> num_;
                if (std::cin.peek() != '\n') {
                    std::string str;
                    std::cin >> str;
                    std::cout << "Unexpected: '" << str << "'" <<std::endl;
                    cmd_ = Command::INVALID;
                }
                break;

            case 'b':
                cmd_ = Command::SET_BREAKPOINT;
                std::cin >> std::hex >> num_ >> std::dec;
                if (std::cin.peek() != '\n') {
                    std::string str;
                    std::cin >> str;
                    std::cout << "Unexpected: '" << str << "'" <<std::endl;
                    cmd_ = Command::INVALID;
                }
                break;

            default:
                cmd_ = Command::INVALID;
                break;
        }
    }

    auto GetCmd()
    {
        return cmd_;
    }
    
    auto GetNum()
    {
        return num_;
    }


private:
    Command cmd_;
    size_t num_;
};

} // namespace sim
