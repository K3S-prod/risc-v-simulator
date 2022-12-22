
#include <elfio/elfio.hpp>
#include <string>

#include "cpu.h"
#include "simulator.h"

namespace sim {

Simulator::Simulator(std::string& elfFileName): loader_(elfFileName), memory_(loader_) {}

void Simulator::runSimulation() {
    return;
}

} // namespace sim
