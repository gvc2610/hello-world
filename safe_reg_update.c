/**
 * driver_solution.c
 * * Problem: Safely update bits [16:14] of a shared hardware register.
 * Context: Pre-Silicon / Switch Firmware
 * 
Problem Statement: Safe Register Modification
Scenario: You are writing a firmware driver for the Scorpio Switch. You must implement a function to update a specific 3-bit field within a 32-bit Memory Mapped Register (MMR).

Specifications:

Register Address: 0x40001000

Target Field: Bits (3 bits wide).

Input: A value (0-7) to write into this field.

Constraints:

The register contains other critical control bits that must not be corrupted.

The function works in a concurrent environment: It can be called from a thread, but the same register is also modified by high-frequency Interrupt Service Routines (ISRs).

The system uses an ARM-based controller (assumed for locking logic).
 */

#include <stdint.h>
#include <errno.h>  // Standard error codes (e.g., -EINVAL)
#include <stdbool.h>

// ============================================================================
// 1. Hardware Definitions
// ============================================================================

// Physical address of the register
#define MMR_REG_ADDR  0x40001000

// Mask for bits [16:14]. 
// Binary: 0000 0000 0000 0001 1100 0000 0000 0000
// Hex:    0x0001C000
#define MMR_FIELD_MASK  (0x7UL << 14)

// Macro for Register Access
// 'volatile' is CRITICAL. It tells the compiler:
// "Do not optimize reads/writes to this address. Always go to hardware."
// We cast to uint32_t* to ensure we generate a 32-bit bus transaction.
#define REG32(addr)     (*(volatile uint32_t *)(addr))


// ============================================================================
// 2. System Abstraction Layer (Mocking OS/Arch Primitives)
// ============================================================================

// In a real RTOS (FreeRTOS, ThreadX) or Linux kernel, these would be provided.
// We declare them here to demonstrate "Principal" awareness of nesting.

typedef unsigned long irq_flags_t;

// SAVES the current interrupt state (on/off) into flags and then disables interrupts.
// This supports nested locking: if interrupts were already off, we remember that.
extern void irq_save(irq_flags_t *flags);

// RESTORES the interrupt state from flags. 
// If they were off before, they stay off. If they were on, they turn back on.
extern void irq_restore(irq_flags_t flags);


// ============================================================================
// 3. The Solution
// ============================================================================

/**
 * UpdateMMRField - Updates bits [16:14] of the MMR register.
 * * @param val: The value (0-7) to write.
 * @return:    0 on success, negative error code on failure.
 * * ARCHITECTURAL NOTES:
 * 1. Concurrency: Uses irq_save/restore because standard Mutexes do not 
 * protect against ISRs (Priority Inversion/Deadlock risk).
 * 2. Atomicity: The lock covers the ENTIRE Read-Modify-Write sequence.
 * 3. Latency: Critical section is kept minimal to reduce interrupt latency.
 */
int UpdateMMRField(uint32_t val) {
    uint32_t reg_val;
    irq_flags_t flags;

    // ---------------------------------------------------------
    // Step 1: Input Validation
    // ---------------------------------------------------------
    // Principal Insight: Never trust the caller. Check bounds immediately.
    // Do NOT use printf() in drivers; it's slow and non-reentrant.
    if (val > 7) {
        return -EINVAL; 
    }

    // ---------------------------------------------------------
    // Step 2: Enter Critical Section (The "Snapshot")
    // ---------------------------------------------------------
    // We disable interrupts to prevent an ISR from preempting us 
    // and modifying this register while we are calculating.
    // We use irq_save() instead of irq_disable() to respect the caller's state.
    irq_save(&flags);

    // ---------------------------------------------------------
    // Step 3: The Read-Modify-Write (RMW) Sequence
    // ---------------------------------------------------------
    
    // A. READ: Get the fresh current state from hardware.
    reg_val = REG32(MMR_REG_ADDR);

    // B. MODIFY: 
    // First, clear the target bits using the inverted mask.
    // reg_val: xxxxxxxx xxxxxxxx x000xxxx xxxxxxxx
    reg_val &= ~MMR_FIELD_MASK;

    // Next, logical OR the new value (shifted to position 14).
    // reg_val: xxxxxxxx xxxxxxxx xVALxxxx xxxxxxxx
    reg_val |= (val << 14);

    // C. WRITE: Commit the new value back to hardware.
    // Because we are inside the critical section, we are guaranteed
    // that no ISR has touched the register since our READ.
    REG32(MMR_REG_ADDR) = reg_val;

    // ---------------------------------------------------------
    // Step 4: Exit Critical Section
    // ---------------------------------------------------------
    // Restore interrupts to exactly how we found them.
    irq_restore(flags);

    return 0;
}

// ============================================================================
// 4. Test Harness (Stub for compilation)
// ============================================================================

// Mock implementations for compilation purposes
void irq_save(irq_flags_t *flags) { *flags = 1; /* Implementation hidden */ }
void irq_restore(irq_flags_t flags) { /* Implementation hidden */ }

int main(void) {
    // Example usage
    int ret = UpdateMMRField(5);
    if (ret < 0) {
        // Handle error
    }
    return 0;
}