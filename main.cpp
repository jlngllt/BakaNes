#include <iostream>
#include <cstdint>
#include <cassert>
#include <array>
#include <vector>

#include <stdio.h>

using namespace std;

#define ARRAY_LEN(x) (sizeof(x) / sizeof(*x))
static void TODO() { assert(false);}

namespace emulator {

class Ines {
    public:
        enum rom_section {
            SECTION_HEADER = 1 << 0,
        };
        Ines(std::string const &filename) : 
            m_filename(filename),
            m_file_handler(NULL) {}
        bool Open();
        bool Close();
        void Print(rom_section sections);
        bool Parse(uint8_t *buffer);
    public:
        union {
            uint8_t buffer[16];
            struct {
                uint8_t NES[3];
                uint8_t prg_rom_size;
                uint8_t chr_rom_size;
                uint8_t flag_6;
                uint8_t flag_7;
                uint8_t flag_8;
                uint8_t flag_9;
                uint8_t flag_10;
            } fmt ;
        } m_u_header;
        std::array<uint8_t, 16> m_header;
        std::array<uint8_t, 512> m_trainer;
        std::vector<uint8_t> m_rom_data;
        std::vector<uint8_t> m_inst_rom;
        std::array<uint8_t, 16> m_prom_data;
        std::array<uint8_t, 16> m_prom_counterout;
        uint32_t m_rom_len;
    private:
        FILE *m_file_handler;
        std::string const m_filename;

};

bool Ines::Open() {
    FILE *fp = fopen(m_filename.c_str(), "rb");
    if (!fp) return false;
    m_file_handler = fp;
    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    printf("Open: %s (%ld)\n", m_filename.c_str(), len);
    m_rom_len = len;
    return true;
}

bool Ines::Parse(uint8_t *buffer) {
    int n_read = fread(buffer, 1, m_rom_len, m_file_handler);
    printf("Read %d bytes\n", n_read);
    if (n_read != m_rom_len) return false;

    for (int i = 0; i < ARRAY_LEN(m_u_header.buffer) && i < n_read; i++) {
        m_u_header.buffer[i] = buffer[i];
        printf("[%02x]", m_u_header.buffer[i]);
    }
    printf("\n");

    // TODO: Other section
    return true;
}

void Ines::Print(rom_section section) {
    switch(section) {
        case SECTION_HEADER: {
            printf("INES         = %c%c%c\n", m_u_header.fmt.NES[0], m_u_header.fmt.NES[1], m_u_header.fmt.NES[2]);
            printf("PRG ROM size = %d\n", m_u_header.fmt.prg_rom_size);
            printf("CHR ROM size = %d\n", m_u_header.fmt.chr_rom_size);
            printf("flag 6       = %d\n", m_u_header.fmt.flag_6);
            printf("flag 7       = %d\n", m_u_header.fmt.flag_7);
            printf("flag 8       = %d\n", m_u_header.fmt.flag_8);
            printf("flag 9       = %d\n", m_u_header.fmt.flag_9);
            printf("flag 10      = %d\n", m_u_header.fmt.flag_10);
        } break;
    }

}

bool Ines::Close() {
    printf("Close file: %s\n", m_filename.c_str());
    return fclose(m_file_handler) == 0;
}

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

int main(int argc, char **argv) {
    emulator::Ines rom("rom/mario_bros.nes");
    rom.Open();
    uint8_t *buffer = (uint8_t *)malloc(sizeof(uint8_t) * rom.m_rom_len);
    rom.Parse(buffer);
    rom.Print(emulator::Ines::SECTION_HEADER);

    return 0;
}   