#include "memory_system.h"

namespace dramsim3 {
MemorySystem::MemorySystem(const std::string &config_file,
                           const std::string &output_dir,
                           std::function<void(uint64_t)> read_callback,
                           std::function<void(uint64_t)> write_callback)
    : config_(new Config(config_file, output_dir)) {
    // TODO: ideal memory type?
    if (config_->IsHMC()) {
        dram_system_ = new HMCMemorySystem(*config_, output_dir, read_callback,
                                           write_callback);
    } else {
        dram_system_ = new JedecDRAMSystem(*config_, output_dir, read_callback,
                                           write_callback);
    }
}

MemorySystem::~MemorySystem() {
    delete (dram_system_);
    delete (config_);
}

void MemorySystem::ClockTick() { dram_system_->ClockTick(); }

double MemorySystem::GetTCK() const { return config_->tCK; }

int MemorySystem::GetBusBits() const { return config_->bus_width; }

int MemorySystem::GetBurstLength() const { return config_->BL; }

int MemorySystem::GetQueueSize() const { return config_->trans_queue_size; }

int MemorySystem::GetChannel(uint64_t hex_addr) const {
    return dram_system_->GetChannel(hex_addr);
}

int MemorySystem::GetRank(uint64_t hex_addr) const {
    return dram_system_->GetRank(hex_addr);
}

int MemorySystem::GetBank(uint64_t hex_addr) const {
    return dram_system_->GetBank(hex_addr);
}

int MemorySystem::GetNumChannel() const {
    return dram_system_->GetNumChannel();
}

int MemorySystem::GetNumRank() const {
    return dram_system_->GetNumRank();
}

int MemorySystem::GetNumBank() const {
    return dram_system_->GetNumBank();
}

void MemorySystem::RegisterCallbacks(
    std::function<void(uint64_t)> read_callback,
    std::function<void(uint64_t)> write_callback) {
    dram_system_->RegisterCallbacks(read_callback, write_callback);
}

bool MemorySystem::WillAcceptTransaction(uint64_t hex_addr,
                                         bool is_write) const {
    return dram_system_->WillAcceptTransaction(hex_addr, is_write);
}

bool MemorySystem::WillAcceptTransactionByChannel(int channel_id,
        bool is_write) const {
    return dram_system_->WillAcceptTransactionByChannel(channel_id, is_write);
}

bool MemorySystem::AddTransaction(uint64_t hex_addr, bool is_write,
        bool priority) {
    return dram_system_->AddTransaction(hex_addr, is_write, priority);
}

void MemorySystem::PrintStats() const { dram_system_->PrintStats(); }

void MemorySystem::ResetStats() { dram_system_->ResetStats(); }

MemorySystem* GetMemorySystem(const std::string &config_file, const std::string &output_dir,
                 std::function<void(uint64_t)> read_callback,
                 std::function<void(uint64_t)> write_callback) {
    return new MemorySystem(config_file, output_dir, read_callback, write_callback);
}
}  // namespace dramsim3

// This function can be used by autoconf AC_CHECK_LIB since
// apparently it can't detect C++ functions.
// Basically just an entry in the symbol table
extern "C" {
void libdramsim3_is_present(void) { ; }
}
