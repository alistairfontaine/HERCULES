#include "../include/HerculesCore.hpp"
#include <iostream>
#include <cstring>

int main() {
    std::cout << "HERCULES Passive Threat Deflector Core Initiated\n";
    std::cout << "------------------------------------------------\n";

    Hercules::DeflectorEngine engine;

    // Simulate standard, steady execution stack states
    Hercules::ExecutionSlot stableCycle1{0x7FFF1000, 0x0120, 64, "kernel_main"};
    Hercules::ExecutionSlot stableCycle2{0x7FFF1004, 0x0124, 64, "kernel_main"};

    std::cout << "[Monitor] Analyzing stable system execution sequences...\n";
    if (engine.inspect_execution_slot(stableCycle1)) std::cout << " -> Cycle 1 cleared.\n";
    if (engine.inspect_execution_slot(stableCycle2)) std::cout << " -> Cycle 2 cleared.\n";

    // Simulate an aggressive zero-day attack vector injection
    // Exploit skews the address pointer and forces an oversized allocation footprint
    std::cout << "\n[Monitor] Simulating zero-day shellcode buffer overflow exploit injection...\n";
    Hercules::ExecutionSlot exploitPayload;
    exploitPayload.instructionPointer = 0x90909090; // Classic NOP Sled injection signature
    exploitPayload.stackOffset = 0xF990;           // Corrupted stack boundary
    exploitPayload.allocationSize = 4096;          // Massive buffer footprint explosion

    // Bounded string copy to safely populate the label buffer context
    std::snprintf(exploitPayload.processLabel, sizeof(exploitPayload.processLabel), "malicious_payload");


    bool integrityCleared = engine.inspect_execution_slot(exploitPayload);

    std::cout << "\n[Metrics] Deflector Operations Summary:\n";
    std::cout << " -> Threat Mitigation State: " << (!integrityCleared ? "SUCCESS (Exploit Contained)" : "FAILURE") << "\n";
    std::cout << " -> Confirmed Secure Tracks: " << engine.get_tracked_slots_count() << " allocation blocks preserved.\n";

    // Perform Phase 2 persistence round-trip check using dedicated tests directory mapping
    std::cout << "\n[Serialization] Exporting timeline metrics down to .herc storage matrix...\n";
    try {
        engine.export_to_binary("tests/exploit_log.herc");

        Hercules::DeflectorEngine secondaryRecoveryMatrix;
        secondaryRecoveryMatrix.load_from_binary("tests/exploit_log.herc");
        std::cout << " -> Recovered Integrity Tracks: " << secondaryRecoveryMatrix.get_tracked_slots_count() << " blocks verified.\n";
    } catch (const std::exception& e) {
        std::cerr << " -> Serialization Persistence Defect: " << e.what() << "\n";
        return 1;
    }

    return !integrityCleared ? 0 : 1;
}

