#pragma once

#include "controller.h"
#include "simulator/memory/mmu.h"
#include "simulator/interpreter/interpreter.h"
#include "simulator/decoder/decoder.h"

#include <string>
#include <iomanip>
#include <unordered_set>
#include <chrono>

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
        Decoder debug_decoder;

        controller_.RequireCommand();
        StartTimer();
        while (true) {
            switch (controller_.GetCmd())
            {
                case Controller::Command::NEXT:{
                    VAddr pc_va = interp_.GetPc();
                    auto inst_ptr = static_cast<const uint8_t *>(mmu_.GetNativePointer(pc_va));
                    switch (debug_decoder.Decode(inst_ptr))
                    {
                        case Decoder::DispatchLabel::I_JALR: {
                            // Check if JALR isn't a return:
                            if (debug_decoder.i.rd == 0) {
                                interp_.SetNumInstsToExec(1);
                                interp_.InvokeBreakpointed();
                            } else {
                                uint32_t bva = pc_va.ToReg() + 4U;
                                temp_breakpoint_ = bva;
                                saved_sp_ = interp_.GetReg(2U);
                                interp_.SetNumInstsToExec(-1);
                                interp_.InvokeBreakpointed();
                                saved_sp_ = 0;
                                temp_breakpoint_ = 0;
                            }
                            break;
                        }
                        case Decoder::DispatchLabel::J_JAL: {
                            uint32_t bva = pc_va.ToReg() + 4U;
                            temp_breakpoint_ = bva;
                            saved_sp_ = interp_.GetReg(2U);
                            interp_.SetNumInstsToExec(-1);
                            interp_.InvokeBreakpointed();
                            saved_sp_ = 0;
                            temp_breakpoint_ = 0;
                            break;
                        }
                        case Decoder::DispatchLabel::I_EBREAK: {
                            return;
                        }
                        default: {
                            interp_.SetNumInstsToExec(1);
                            interp_.InvokeBreakpointed();
                        }
                    }
                    break;
                }

                case Controller::Command::STEP: {
                    interp_.SetNumInstsToExec(1U);
                    interp_.InvokeBreakpointed();
                    break;
                }

                case Controller::Command::STEP_SEVERAL: {
                    interp_.SetNumInstsToExec(controller_.GetNum());
                    interp_.InvokeBreakpointed();
                    break;
                }

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
                case Controller::Command::DEL_BREAKPOINT: {
                    uint32_t bva = controller_.GetNum();
                    if (breakpoints_.erase(bva)) {
                        std::cout << "Del breakpoint at 0x" << std::hex << std::setfill('0') << std::setw(8) << bva << std::dec << std::endl;
                    } else {
                        std::cout << "No such breakpoint (0x" << std::hex << std::setfill('0') << std::setw(8) << bva << std::dec << ")" << std::endl;
                    }
                    break;
                }


                case Controller::Command::INVALID:
                    std::cout << "Unknown command" <<std::endl;
                    break;

                default:
                    UNREACHABLE();
            }
            controller_.RequireCommand();
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
    bool IsBreakpointForCmdNext(uint32_t va, uint32_t cur_sp)
    {
        if (cur_sp == saved_sp_) {
            return temp_breakpoint_ == va;
        }
        return false;
    }

    void StartTimer()
    {
        timestamp_start_ = std::chrono::high_resolution_clock::now();
    }
    void StopTimer()
    {
        timestamp_end_ = std::chrono::high_resolution_clock::now();
    }
    double GetTimeMS()
    {
        std::chrono::duration<double, std::milli> dt = timestamp_end_ - timestamp_start_;
        return dt.count();
    }

private:
    MMU mmu_;
    Interpreter interp_;
    std::unordered_set<uint32_t> breakpoints_;
    uint32_t temp_breakpoint_;
    uint32_t saved_sp_;
    Controller controller_;
    std::chrono::time_point<std::chrono::high_resolution_clock> timestamp_start_;
    std::chrono::time_point<std::chrono::high_resolution_clock> timestamp_end_;
};

} // namespace sim
