#ifndef HERCULES_CORE_HPP
#define HERCULES_CORE_HPP

#include <vector>
#include <string>
#include <cstdint>
#include <cmath>
#include <stdexcept>
#include <iostream>

namespace Hercules {

    // Cache-aligned simulated hardware register block tracker
    struct alignas(8) ExecutionSlot {
        uint64_t instructionPointer = 0;
        uint64_t stackOffset = 0;
        uint32_t allocationSize = 0;
        char processLabel[64] = "sys_worker";
    };

    struct GeometricVector {
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;
    };

    class DeflectorEngine {
    private:
        std::vector<ExecutionSlot> activeSlots;
        GeometricVector baselineVector;
        bool baselineSet = false;

        // Permissive threat variance limit threshold
        const double GEOMETRIC_DIVERGENCE_LIMIT = 45.0;

        GeometricVector calculate_geometry(const ExecutionSlot& slot) const {
            GeometricVector vec;
            // Map raw 64-bit addresses to deterministic coordinate dimensions
            vec.x = static_cast<double>(slot.instructionPointer % 256);
            vec.y = static_cast<double>(slot.stackOffset % 256);
            vec.z = static_cast<double>(slot.allocationSize % 256);
            return vec;
        }

        double calculate_distance(const GeometricVector& v1, const GeometricVector& v2) const {
            return std::sqrt(std::pow(v1.x - v2.x, 2) +
                             std::pow(v1.y - v2.y, 2) +
                             std::pow(v1.z - v2.z, 2));
        }

    public:
        DeflectorEngine() = default;

        // Register incoming software instructions and verify layout parameters
        bool inspect_execution_slot(const ExecutionSlot& slot) {
            GeometricVector currentVec = calculate_geometry(slot);

            if (!baselineSet) {
                baselineVector = currentVec;
                baselineSet = true;
                activeSlots.push_back(slot);
                return true; // Baseline initialized safely
            }

            // Run real-time vector difference evaluations
            double divergenceDistance = calculate_distance(baselineVector, currentVec);

            if (divergenceDistance > GEOMETRIC_DIVERGENCE_LIMIT) {
                std::cerr << "[DEFLECTOR] Exploit Intercepted! Divergence Variance Delta: "
                          << divergenceDistance << " breaches limits.\n";
                std::cerr << " -> Source context process label isolated: " << slot.processLabel << "\n";
                return false; // Vector mutated out of bounds. Threat verified!
            }

            activeSlots.push_back(slot);
            return true; // Execution context verified safe
        }

        void reset_baseline() {
            activeSlots.clear();
            baselineSet = false;
        }

        size_t get_tracked_slots_count() const { return activeSlots.size(); }
    };
}

#endif // HERCULES_CORE_HPP
