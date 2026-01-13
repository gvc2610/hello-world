#include <stdint.h>
#include <errno.h> // For error codes

#define MMR_REG_ADDR  0x40001000
#define MMR_REG_MASK  (0x7 << 14)

// Use a macro or inline function for the register access to ensure volatile correctness everywhere
#define REG32(addr)   (*(volatile uint32_t *)(addr))

int UpdateMMRReg(uint32_t val) {
    uint32_t reg_val;
    unsigned long flags; // Variable to save interrupt state

    // 1. Validation (Return Error Code, NO printf)
    if (val > 7) {
        return -EINVAL; 
    }

    // 2. CRITICAL SECTION START
    // We don't just disable; we SAVE the current state. 
    // If interrupts were ALREADY disabled by the caller, we shouldn't re-enable them later!
    irq_save(flags); 

    // 3. The Operation (Read-Modify-Write)
    reg_val = REG32(MMR_REG_ADDR);            // READ
    reg_val &= ~MMR_REG_MASK;                 // MODIFY (Clear)
    reg_val |= (val << 14);                   // MODIFY (Set)
    REG32(MMR_REG_ADDR) = reg_val;            // WRITE

    // 4. CRITICAL SECTION END
    irq_restore(flags); // Restore previous state

    return 0;
}
/*
The ISR is technically free to try to update the register, 
but your irq_save forces it to wait until your Read-Modify-Write transaction is complete, 
ensuring data integrity.
*/