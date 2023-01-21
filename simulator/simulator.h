#pragma once

#include "controller.h"
#include "simulator/memory/mmu.h"
#include "simulator/interpreter/interpreter.h"

#include <string>
#include <iomanip>
#include <unordered_set>

namespace sim {

class Simulator {

public:
    Simulator(const std::string& elfFileName) : mmu_(elfFileName), interp_(this, &mmu_) {}
    void runSimulation()
    {
        constexpr uint32_t SP = 0xBFFFFFFF;
        constexpr uint32_t STACK_SIZE_PAGES = 1;
        if (!AllocStack(STACK_SIZE_PAGES, SP)) {
            LOG_FATAL(SIMULATOR, "Failed to allocate stack");
        }

        constexpr uint8_t SP_REG_IDX = 2U;
        interp_.SetReg(SP_REG_IDX, SP);
        interp_.SetPc(mmu_.GetEntryPoint().ToReg());
        //interp_.SetInstsToExecNum(1U);

        while (true) {
            controller_.RequireCommand();
            switch (controller_.GetCmd())
            {
                case Controller::Command::NEXT:
                    interp_.SetNumInstsToExec(1);
                    interp_.InvokeBreakpointed();
                    break;

                case Controller::Command::NEXT_SEVERAL:
                    interp_.SetNumInstsToExec(controller_.GetNum());
                    interp_.InvokeBreakpointed();
                    break;

                case Controller::Command::CONTINUE:
                    if (breakpoints_.empty()) {
                        interp_.Invoke();
                        return;
                    }
                    interp_.SetNumInstsToExec(-1);
                    interp_.InvokeBreakpointed();
                    break;

                case Controller::Command::QUIT:
                    return;

                case Controller::Command::REG_PRINT: {
                    auto reg_id = controller_.GetNum();
                    std::cout << "x" << reg_id << " (" << Register::GetRegName(reg_id) << "): ";
                    std::cout << "0x" << std::hex << std::setfill('0') << std::setw(8) << interp_.GetReg(reg_id) << std::dec;
                    std::cout << "    " << interp_.GetReg(reg_id) << std::endl;
                    break;
                }

                case Controller::Command::SET_BREAKPOINT: {
                    uint32_t bva = controller_.GetNum();
                    breakpoints_.insert(bva);
                    std::cout << "Set breakpoint at 0x" << std::hex << std::setfill('0') << std::setw(8) << bva << std::dec << std::endl;
                    break;
                }

                case Controller::Command::INVALID:
                    std::cout << "Unknown command" <<std::endl;
                    break;

                default:
                    UNREACHABLE();
            }
        }
    }

    bool AllocStack(size_t n_pages, size_t sp)
    {
        for (size_t i = 0; i < n_pages; i++) {
            uint32_t page_begin_va = sp - PAGE_SIZE * (i + 1U) + 1U;
            if (mmu_.AllocPage(page_begin_va) == nullptr) {
                return false;
            }
        }
        return true;
    }
    bool IsBreakpoint(uint32_t va)
    {
        return breakpoints_.find(va) != breakpoints_.end();
    }

private:
    MMU mmu_;
    Interpreter interp_;
    std::unordered_set<uint32_t> breakpoints_;
    Controller controller_;
};

} // namespace sim
