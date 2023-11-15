#include <iostream>
#include <cstdint>
#include <cassert>
#include <array>
#include <vector>

#include <stdio.h>

using namespace std;

#define ARRAY_LEN(x) (sizeof(x) / sizeof(*x))
#define TODO assert(false);

namespace emulator {

class Ines {
    public:
        enum rom_section {
            SECTION_HEADER = 1,
            SECTION_PRG_ROM,
            SECTION_CHR_ROM
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
                uint8_t NES[4];
                uint8_t prg_rom_size;
                uint8_t chr_rom_size;
                // uint8_t flag_6;
                union {
                    uint8_t data;
                    struct {
                        uint8_t mirroring: 1; // 0 horizontal (vertical arrangement); 1 vertical(horizontal arrangement)
                        uint8_t cartdridge_contains_battery:1;
                        uint8_t trainer_512_bytes: 1;
                        uint8_t ignore_mirroring_control: 1;
                        uint8_t lower_nybble_mapper_number: 4;
                    } fmt;
                } flag_6;
                uint8_t flag_7;
                uint8_t flag_8;
                uint8_t flag_9;
                uint8_t flag_10;
            } fmt ;
        } m_u_header;
        std::array<uint8_t, 16> m_header;
        std::array<uint8_t, 512> m_trainer;
        std::vector<uint8_t> m_prg_rom_data;
        std::vector<uint8_t> m_chr_rom_data;
        std::vector<uint8_t> m_playchoice_inst_rom; // not used;
        std::array<uint8_t, 16> m_playchoice_prom_data; // not used
        std::array<uint8_t, 16> m_playchoice_prom_counterout; // not used

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

    int idx = 0;
    for (int j_header = 0; j_header < 16; j_header++, idx++) {
        m_u_header.buffer[j_header] = buffer[idx];
    }
    
    if (m_u_header.fmt.flag_6.fmt.trainer_512_bytes == 1) {
        for (int j_trainer = 0; j_trainer < 512; j_trainer++, idx++) {
            m_trainer[j_trainer] = buffer[idx];
        }
    }

    for (int j_prg_rom = 0; j_prg_rom < m_u_header.fmt.prg_rom_size * 16384; j_prg_rom++, idx++) {
        m_prg_rom_data.push_back(buffer[idx]);
    }

    for (int j_chr_rom = 0; j_chr_rom < m_u_header.fmt.chr_rom_size * 8192; j_chr_rom++, idx++) {
        m_chr_rom_data.push_back(buffer[idx]);
    }
    
    return true;
}

void Ines::Print(rom_section section) {
    switch(section) {
        case SECTION_HEADER: {
            printf("HEADER:::\n");
            printf("INES         = %c%c%c[%02x]\n", m_u_header.fmt.NES[0], m_u_header.fmt.NES[1], m_u_header.fmt.NES[2], m_u_header.fmt.NES[3]);
            printf("PRG ROM size = %d\n", m_u_header.fmt.prg_rom_size);
            printf("CHR ROM size = %d\n", m_u_header.fmt.chr_rom_size);
            printf("flag 6 = %d\n", m_u_header.fmt.flag_6.data);
            printf("  lower nybble mapper number = %d\n", m_u_header.fmt.flag_6.fmt.lower_nybble_mapper_number);
            printf("  ignore mirroring control = %d\n", m_u_header.fmt.flag_6.fmt.ignore_mirroring_control);
            printf("  trainer 512 bytes = %d\n", m_u_header.fmt.flag_6.fmt.trainer_512_bytes);
            printf("  cartdridge contains battery = %d\n", m_u_header.fmt.flag_6.fmt.cartdridge_contains_battery);
            printf("  mirroring = %d\n", m_u_header.fmt.flag_6.fmt.mirroring);
            printf("flag 7       = %d\n", m_u_header.fmt.flag_7);
            printf("flag 8       = %d\n", m_u_header.fmt.flag_8);
            printf("flag 9       = %d\n", m_u_header.fmt.flag_9);
            printf("flag 10      = %d\n", m_u_header.fmt.flag_10);
        } break;

        case SECTION_PRG_ROM: {
            printf("PRG ROM:::\n");
            for (int i = 0; i < 1 /*m_prg_rom_data.size() */; i++) {
                printf("[%02x]", m_prg_rom_data[i]);
            }
            printf("\n");
        } break;

        case SECTION_CHR_ROM: {
            printf("CHR ROM:::\n");
            for (int i = 0; i < m_chr_rom_data.size(); i++) {
                printf("[%02x]", m_chr_rom_data[i]);
            }
            printf("\n");
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
    struct {
        uint8_t C : 1; // Carry
        uint8_t Z: 1; // Zero
        uint8_t I: 1; // Interrupt disable
        uint8_t D: 1; // Decimal
        uint8_t spare_b : 1; // no cpu effect
        uint8_t spare_1 : 1; // no cpu effect : 1
        uint8_t V: 1; // Overflow
        uint8_t N: 1; // Negative
    } P;
};


class CPU {
public:
    // TODO add cast there
    CPU(std::vector<uint8_t> /*const*/ &prg_rom_data) : m_prg_rom_data(prg_rom_data) {}
    void loop();

    private:
    void op_adc() {  TODO; } // Add with Carry
    void op_and() {  TODO; } // Logical AND
    void op_asl() {  TODO; } // Arithmetic Shift Left
    void op_bcc() {  TODO; } // Branch if Carry Clear
    void op_bcs() {  TODO; } // Branch if Carry Set
    void op_beq() {  TODO; } // Branch if Equal
    void op_bit() {  TODO; } // Bit Test
    void op_bmi() {  TODO; } // Branch if Minus
    void op_bne() {  TODO; } // Branch if Not Equal
    void op_bpl() {  TODO; } // Branch if Positive
    void op_brk() {  TODO; } // Force Interrupt
    void op_bvc() {  TODO; } // Branch if Overflow Clear
    void op_bvs() {  TODO; } // Branch if Overflow Set
    void op_clc() {  TODO; } // Clear Carry Flag
    // Clear Decimal Mode
    void op_cld() { // 0xD8
        m_register.P.D = 0;
    } 
    void op_cli() {  TODO; } // Clear Interrupt Disable
    void op_clv() {  TODO; } // Clear Overflow Flag
    void op_cmp() {  TODO; } // Compare
    void op_cpx() {  TODO; } // Compare X Register
    void op_cpy() {  TODO; } // Compare Y Register
    void op_dec() {  TODO; } // Decrement Memory
    void op_dex() {  TODO; } // Decrement X Register
    void op_dey() {  TODO; } // Decrement Y Register
    void op_eor() {  TODO; } // Exclusive OR
    void op_inc() {  TODO; } // Increment Memory
    void op_inx() {  TODO; } // Increment X Register
    void op_iny() {  TODO; } // Increment Y Register
    void op_jmp() {  TODO; } // Jump
    void op_jsr() {  TODO; } // Jump to Subroutine
    void op_lda() {  TODO; } // Load Accumulator
    void op_ldx() {  TODO; } // Load X Register
    void op_ldy() {  TODO; } // Load Y Register
    void op_lsr() {  TODO; } // Logical Shift Right
    void op_nop() {  TODO; } // No Operation
    void op_ora() {  TODO; } // Logical Inclusive OR
    void op_pha() {  TODO; } // Push Accumulator
    void op_php() {  TODO; } // Push Processor Status
    void op_pla() {  TODO; } // Pull Accumulator
    void op_plp() {  TODO; } // Pull Processor Status
    void op_rol() {  TODO; } // Rotate Left
    void op_ror() {  TODO; } // Rotate Right
    void op_rti() {  TODO; } // Return from Interrupt
    void op_rts() {  TODO; } // Return from Subroutine
    void op_sbc() {  TODO; } // Subtract with Carry
    void op_sec() {  TODO; } // Set Carry Flag
    void op_sed() {  TODO; } // Set Decimal Flag
    void op_sei() {  TODO; } // Set Interrupt Disable
    void op_sta() {  TODO; } // Store Accumulator
    void op_stx() {  TODO; } // Store X Register
    void op_sty() {  TODO; } // Store Y Register
    void op_tax() {  TODO; } // Transfer Accumulator to X
    void op_tay() {  TODO; } // Transfer Accumulator to Y
    void op_tsx() {  TODO; } // Transfer Stack Pointer to X
    void op_txa() {  TODO; } // Transfer X to Accumulator
    void op_txs() {  TODO; } // Transfer X to Stack Pointer
    void op_tya() {  TODO; } // Transfer Y to Accumulator
    
private:
    Register m_register;
    std::vector<uint8_t> const &m_prg_rom_data;
};

void CPU::loop() {
    int idx = 0; // just for the test
    while(1) {
        printf("m_prg_rom[%d] = [%02x]\n" , idx, m_prg_rom_data[idx]);
        switch(m_prg_rom_data[idx]) {
            case 0x00: op_brk(); break;
            case 0xD8: op_cld(); break;
            default:  TODO;
        }
        idx++;
    }
}

}



int main(int argc, char **argv) {
    emulator::Ines rom("rom/mario_bros.nes");
    rom.Open();
    uint8_t *buffer = (uint8_t *)malloc(sizeof(uint8_t) * rom.m_rom_len);
    rom.Parse(buffer);
    rom.Print(emulator::Ines::SECTION_PRG_ROM);
    emulator::CPU cpu(rom.m_prg_rom_data);
    cpu.loop();
    return 0;
}   