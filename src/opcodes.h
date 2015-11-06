// *** Referenced https://github.com/haldean/x6502 ***
// Opcodes that use multiple addressing modes are named in the form of
// I_A, where I is the instruction and A is the addressing mode. All
// possible addressing modes are:
//      AB:  absolute mode, next two bytes are the low/high byte of an absolute
//           memory address
//      ABX: absolute,X, next two bytes are added to the value in register X to
//           get the memory address
//      ABY: same as ABX, except the value of register Y is used instead of X
//      ACC: accumulator, act on the value in the accumulator
//      IMM: immediate, next byte is a constant to be used instead of a lookup
//      IN:  indirect, next two bytes are an absolute memory address of the
//           lower nibble of a memory address. That byte and the byte after will
//           be loaded and the address made of those two bytes will be used
//      INX: (indirect,X) mode, add X to the following byte, modulo 0xFF, and
//           lookup two bytes starting at that location. Those two bytes form
//           the memory address that will be used
//      INY: (indirect),Y mode, look up two bytes starting at address in the
//           following byte, add Y modulo 0xFFFF, and use the result as an
//           address
//      REL: relative, next byte contains a signed offset from the current PC.
//      ZP:  zero-page, next byte is the low bits of the memory address (can
//           only address first 256 bytes of memory using ZP)
//      ZPX: zero-page,X, add next byte to X modulo 0xFF and use that as a
//           memory address

#ifndef __6502_opcodes__
#define __6502_opcodes__

#define ADC_IMM     0x69
#define ADC_ZP      0x65
#define ADC_ZPX     0x75
#define ADC_AB      0x60
#define ADC_ABX     0x70
#define ADC_ABY     0x79
#define ADC_INX     0x61
#define ADC_INY     0x71

#define BNE_REL     0xD0

#define CLC         0x18

#define DEY         0x88

#define LDA_IMM     0xA9
#define LDA_ZP      0xA5
#define LDA_ZPX     0xB5
#define LDA_AB      0xAD
#define LDA_ABX     0xBD
#define LDA_ABY     0xB9
#define LDA_INX     0xA1
#define LDA_INY     0xB1

#define LDX_IMM     0xA2
#define LDX_ZP      0xA6
#define LDX_ZPY     0xB6
#define LDX_AB      0xAE
#define LDX_ABY     0xBE

#define LDY_IMM     0xA0
#define LDY_ZP      0xA4
#define LDY_ZPX     0xB4
#define LDY_AB      0xAC
#define LDY_ABX     0xBC

#define SBC_IMM     0xE9
#define SBC_ZP      0xE5
#define SBC_ZPX     0xF5
#define SBC_AB      0xED
#define SBC_ABX     0xFD
#define SBC_ABY     0xF9
#define SBC_INX     0xE1
#define SBC_INY     0xF1

#define SEC         0x38

#define STA_ZP      0x85
#define STA_ZPX     0x95
#define STA_AB      0x80
#define STA_ABX     0x90
#define STA_ABY     0x99
#define STA_INX     0x81
#define STA_INY     0x91

#define STX_ZP      0x86
#define STX_ZPY     0x96
#define STX_AB      0x8E

#define TAY         0xAB

#define TYA         0x98

#endif
