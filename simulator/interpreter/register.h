#ifndef SIMULATOR_INTERPRETER_REGISTER
#define SIMULATOR_INTERPRETER_REGISTER

#include "simulator/memory/primitives.h"
#include <cstdint>
#include <cstddef>
#include <array>
#include <iomanip>

namespace sim {

class Register
{
public:
    Register() {}

    template <typename T>
    Register(T v) : storage_(v) {}

    static std::string GetRegName(int8_t reg_id)
    {
        switch (reg_id) {
            case 0: return "zero";
            case 1: return "ra";
            case 2: return "sp";
            case 3: return "gp";
            case 4: return "tp";
            case 5: return "t0";
            case 6: return "t1";
            case 7: return "t2";
            case 8: return "s0";
            case 9: return "s1";
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:
            case 17: {
                return std::string("a") + std::to_string(reg_id - 10);
            }
            case 18:
            case 19:
            case 20:
            case 21:
            case 22:
            case 23:
            case 24:
            case 25:
            case 26:
            case 27: {
                return std::string("s") + std::to_string(reg_id - 16);
            }
            case 28:
            case 29:
            case 30:
            case 31: {
                return std::string("t") + std::to_string(reg_id - 25);
            }
            default:
                UNREACHABLE();
        }
    }
    std::string DumpHex() const
    {
        std::stringstream ss;
        ss << "0x" << std::hex << std::setfill('0') << std::setw(8) << storage_;
        return ss.str();
    }

    int32_t operator+(int32_t rhs) const
    {
        return storage_ + rhs;
    }
    uint32_t operator+(uint32_t rhs) const
    {
        return storage_ + rhs;
    }
    uint32_t operator+(const Register &rhs) const
    {
        return storage_ + rhs.storage_;
    }
    uint32_t operator+=(const Register &rhs)
    {
        storage_ += rhs.storage_;
        return storage_;
    }

    operator uint32_t() const
    {
        return storage_;
    }
    operator VAddr() const
    {
        return storage_;
    }
private:
    uint32_t storage_;
};

} // namespace sim

#endif  // SIMULATOR_INTERPRETER_REGISTER
