#include "decoder.h"
#include "simulator/interpreter/register.h"

#include <cstdint>
#include <sstream>

namespace sim {

std::string Decoder::R::Dump(const std::string &prefix)
{
    std::stringstream ss;
    ss << prefix << " (R_type): {\n";
    ss << "    opcode: " << opcode << "\n";
    ss << "    rs1: " << Register::GetRegName(rs1) << "\n";
    ss << "    rs2: " << Register::GetRegName(rs2) << "\n";
    ss << "    rd: " << Register::GetRegName(rd) << "\n";
    ss << "}" << std::endl;
    return ss.str();
}
std::string Decoder::I::Dump(const std::string &prefix)
{
    std::stringstream ss;
    ss << prefix << " (I_type): {\n";
    ss << "    opcode: " << opcode << "\n";
    ss << "}" << std::endl;
    return ss.str();
}
std::string Decoder::S::Dump(const std::string &prefix)
{
    std::stringstream ss;
    ss << prefix << " (S_type): {\n";
    ss << "    opcode: " << opcode << "\n";
    ss << "    rs1: " << Register::GetRegName(rs1) << "\n";
    ss << "    rs2: " << Register::GetRegName(rs2) << "\n";
    ss << "}" << std::endl;
    return ss.str();
}
std::string Decoder::B::Dump(const std::string &prefix)
{
    std::stringstream ss;
    ss << prefix << " (B_type): {\n";
    ss << "    opcode: " << opcode << "\n";
    ss << "}" << std::endl;
    return ss.str();
}
std::string Decoder::U::Dump(const std::string &prefix)
{
    std::stringstream ss;
    ss << prefix << " (U_type): {\n";
    ss << "    opcode: " << opcode << "\n";
    ss << "}" << std::endl;
    return ss.str();
}
std::string Decoder::J::Dump(const std::string &prefix)
{
    std::stringstream ss;
    ss << prefix << " (J_type): {\n";
    ss << "    opcode: " << opcode << "\n";
    ss << "}" << std::endl;
    return ss.str();
}
std::string Decoder::A::Dump(const std::string &prefix)
{
    std::stringstream ss;
    ss << prefix << " (A_type): {\n";
    ss << "    opcode: " << opcode << "\n";
    ss << "}" << std::endl;
    return ss.str();
}

}  // namespace sim::interpreter
