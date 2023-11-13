#include <iostream>
#include <cstdint>
#include <cassert>

using namespace std;

static void TODO() { assert(false);}

namespace emulator {

class Register {
public:
    Register() {}
private:
    // Acumulator
    uint8_t A;
    // Indexes
    uint8_t X;
    uint8_t Y;
    // Program Counter
    uint16_t PC;
    // Stack pointer
    uint8_t SP;
    // Status Register
    struct P {
        uint8_t C : 1; // Carry
        uint8_t Z: 1; // Zero
        uint8_t I: 1; // Interrupt disable
        uint8_t D: 1; // Decimal
        uint8_t spare_b : 1; // no cpu effect
        uint8_t spare_1 : 1; // no cpu effect : 1
        uint8_t V: 1; // Overflow
        uint8_t N: 1; // Negative
    };
};


class CPU {
public:
    CPU() {}

    void op_adc() { TODO(); } // Add with Carry
    void op_and() { TODO(); } // Logical AND
    void op_asl() { TODO(); } // Arithmetic Shift Left
    void op_bcc() { TODO(); } // Branch if Carry Clear
    void op_bcs() { TODO(); } // Branch if Carry Set
    void op_beq() { TODO(); } // Branch if Equal
    void op_bit() { TODO(); } // Bit Test
    void op_bmi() { TODO(); } // Branch if Minus
    void op_bne() { TODO(); } // Branch if Not Equal
    void op_bpl() { TODO(); } // Branch if Positive
    void op_brk() { TODO(); } // Force Interrupt
    void op_bvc() { TODO(); } // Branch if Overflow Clear
    void op_bvs() { TODO(); } // Branch if Overflow Set
    void op_clc() { TODO(); } // Clear Carry Flag
    void op_cld() { TODO(); } // Clear Decimal Mode
    void op_cli() { TODO(); } // Clear Interrupt Disable
    void op_clv() { TODO(); } // Clear Overflow Flag
    void op_cmp() { TODO(); } // Compare
    void op_cpx() { TODO(); } // Compare X Register
    void op_cpy() { TODO(); } // Compare Y Register
    void op_dec() { TODO(); } // Decrement Memory
    void op_dex() { TODO(); } // Decrement X Register
    void op_dey() { TODO(); } // Decrement Y Register
    void op_eor() { TODO(); } // Exclusive OR
    void op_inc() { TODO(); } // Increment Memory
    void op_inx() { TODO(); } // Increment X Register
    void op_iny() { TODO(); } // Increment Y Register
    void op_jmp() { TODO(); } // Jump
    void op_jsr() { TODO(); } // Jump to Subroutine
    void op_lda() { TODO(); } // Load Accumulator
    void op_ldx() { TODO(); } // Load X Register
    void op_ldy() { TODO(); } // Load Y Register
    void op_lsr() { TODO(); } // Logical Shift Right
    void op_nop() { TODO(); } // No Operation
    void op_ora() { TODO(); } // Logical Inclusive OR
    void op_pha() { TODO(); } // Push Accumulator
    void op_php() { TODO(); } // Push Processor Status
    void op_pla() { TODO(); } // Pull Accumulator
    void op_plp() { TODO(); } // Pull Processor Status
    void op_rol() { TODO(); } // Rotate Left
    void op_ror() { TODO(); } // Rotate Right
    void op_rti() { TODO(); } // Return from Interrupt
    void op_rts() { TODO(); } // Return from Subroutine
    void op_sbc() { TODO(); } // Subtract with Carry
    void op_sec() { TODO(); } // Set Carry Flag
    void op_sed() { TODO(); } // Set Decimal Flag
    void op_sei() { TODO(); } // Set Interrupt Disable
    void op_sta() { TODO(); } // Store Accumulator
    void op_stx() { TODO(); } // Store X Register
    void op_sty() { TODO(); } // Store Y Register
    void op_tax() { TODO(); } // Transfer Accumulator to X
    void op_tay() { TODO(); } // Transfer Accumulator to Y
    void op_tsx() { TODO(); } // Transfer Stack Pointer to X
    void op_txa() { TODO(); } // Transfer X to Accumulator
    void op_txs() { TODO(); } // Transfer X to Stack Pointer
    void op_tya() { TODO(); } // Transfer Y to Accumulator
    
private:
    Register m_register;
};

}

int main() {
    cout << "Hello World\n";
}   