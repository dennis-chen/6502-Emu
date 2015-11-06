opcodes = {
    '---': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'ADC': {'SNGL': 'null', 'INDY': '0x71', 'IMM': '0x69', 'INDX': '0x61', 'ZPX': '0x75', 'ABS': '0x6d', 'ABSY': '0x79', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': '0x65', 'ABSX': '0x7d'},
    'AND': {'SNGL': 'null', 'INDY': '0x31', 'IMM': '0x29', 'INDX': '0x21', 'ZPX': '0x35', 'ABS': '0x2d', 'ABSY': '0x39', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': '0x25', 'ABSX': '0x3d'},
    'ASL': {'SNGL': '0x0a', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': '0x16', 'ABS': '0x0e', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': '0x06', 'ABSX': '0x1e'},
    'BCC': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': '0x90', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'BCS': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': '0xb0', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'BEQ': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': '0xf0', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'BIT': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': '0x2c', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': '0x24', 'ABSX': 'null'},
    'BMI': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': '0x30', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'BNE': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': '0xd0', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'BPL': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': '0x10', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'BRK': {'SNGL': '0x00', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'BVC': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': '0x50', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'BVS': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': '0x70', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'CLC': {'SNGL': '0x18', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'CLD': {'SNGL': '0xd8', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'CLI': {'SNGL': '0x58', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'CLV': {'SNGL': '0xb8', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'CMP': {'SNGL': 'null', 'INDY': '0xd1', 'IMM': '0xc9', 'INDX': '0xc1', 'ZPX': '0xd5', 'ABS': '0xcd', 'ABSY': '0xd9', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': '0xc5', 'ABSX': '0xdd'},
    'CPX': {'SNGL': 'null', 'INDY': 'null', 'IMM': '0xe0', 'INDX': 'null', 'ZPX': 'null', 'ABS': '0xec', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': '0xe4', 'ABSX': 'null'},
    'CPY': {'SNGL': 'null', 'INDY': 'null', 'IMM': '0xc0', 'INDX': 'null', 'ZPX': 'null', 'ABS': '0xcc', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': '0xc4', 'ABSX': 'null'},
    'DEC': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': '0xd6', 'ABS': '0xce', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': '0xc6', 'ABSX': '0xde'},
    'DEX': {'SNGL': '0xca', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'DEY': {'SNGL': '0x88', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'EOR': {'SNGL': 'null', 'INDY': '0x51', 'IMM': '0x49', 'INDX': '0x41', 'ZPX': '0x55', 'ABS': '0x4d', 'ABSY': '0x59', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': '0x45', 'ABSX': '0x5d'},
    'INC': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': '0xf6', 'ABS': '0xee', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': '0xe6', 'ABSX': '0xfe'},
    'INX': {'SNGL': '0xe8', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'INY': {'SNGL': '0xc8', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'JMP': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': '0x4c', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': '0x6c', 'ZP': 'null', 'ABSX': 'null'},
    'JSR': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': '0x20', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'LDA': {'SNGL': 'null', 'INDY': '0xb1', 'IMM': '0xa9', 'INDX': '0xa1', 'ZPX': '0xb5', 'ABS': '0xad', 'ABSY': '0xb9', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': '0xa5', 'ABSX': '0xbd'},
    'LDX': {'SNGL': 'null', 'INDY': 'null', 'IMM': '0xa2', 'INDX': 'null', 'ZPX': 'null', 'ABS': '0xae', 'ABSY': '0xbe', 'BRA': 'null', 'ZPY': '0xb6', 'IND': 'null', 'ZP': '0xa6', 'ABSX': 'null'},
    'LDY': {'SNGL': 'null', 'INDY': 'null', 'IMM': '0xa0', 'INDX': 'null', 'ZPX': '0xb4', 'ABS': '0xac', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': '0xa4', 'ABSX': '0xbc'},
    'LSR': {'SNGL': '0x4a', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': '0x56', 'ABS': '0x4e', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': '0x46', 'ABSX': '0x5e'},
    'NOP': {'SNGL': '0xea', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'ORA': {'SNGL': 'null', 'INDY': '0x11', 'IMM': '0x09', 'INDX': '0x01', 'ZPX': '0x15', 'ABS': '0x0d', 'ABSY': '0x19', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': '0x05', 'ABSX': '0x1d'},
    'PHA': {'SNGL': '0x48', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'PHP': {'SNGL': '0x08', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'PLA': {'SNGL': '0x68', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'PLP': {'SNGL': '0x28', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'ROL': {'SNGL': '0x2a', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': '0x36', 'ABS': '0x2e', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': '0x26', 'ABSX': '0x3e'},
    'ROR': {'SNGL': '0x6a', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': '0x76', 'ABS': '0x6e', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': '0x66', 'ABSX': '0x7e'},
    'RTI': {'SNGL': '0x40', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'RTS': {'SNGL': '0x60', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'SBC': {'SNGL': 'null', 'INDY': '0xf1', 'IMM': '0xe9', 'INDX': '0xe1', 'ZPX': '0xf5', 'ABS': '0xed', 'ABSY': '0xf9', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': '0xe5', 'ABSX': '0xfd'},
    'SEC': {'SNGL': '0x38', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'SED': {'SNGL': '0xf8', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'SEI': {'SNGL': '0x78', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'STA': {'SNGL': 'null', 'INDY': '0x91', 'IMM': 'null', 'INDX': '0x81', 'ZPX': '0x95', 'ABS': '0x8d', 'ABSY': '0x99', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': '0x85', 'ABSX': '0x9d'},
    'STX': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': '0x8e', 'ABSY': 'null', 'BRA': 'null', 'ZPY': '0x96', 'IND': 'null', 'ZP': '0x86', 'ABSX': 'null'},
    'STY': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': '0x94', 'ABS': '0x8c', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': '0x84', 'ABSX': 'null'},
    'TAX': {'SNGL': '0xaa', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'TAY': {'SNGL': '0xa8', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'TSX': {'SNGL': '0xba', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'TXA': {'SNGL': '0x8a', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'TXS': {'SNGL': '0x9a', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'TYA': {'SNGL': '0x98', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
    'WDM': {'SNGL': 'null', 'INDY': 'null', 'IMM': '0x42', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': '0x42', 'ABSX': 'null'},
}

X = 0
Y = 0

fileName = input("Select an assembly file: ")
file = open(fileName)
for line in file:
    (command, operand) = line.split(' ', 2)
    if command in opcodes.keys():
        # immediates
        if operand[0] == "#":
            print(opcodes[command]['IMM'][2:], operand[2:])
        else:
            if operand[0] == "$":
                # zero page (not dealing with relative yet)
                if len(operand) == 3:
                    print(opcodes[command]['ZP'][2:], operand[1:])
                elif operand[3] == ",":
                    # zero page X and Y
                    mode = 'ZP' + operand[4].upper()
                    print(opcodes[command][mode][2:], operand[1:3])
                # absolute (need to add conditions once other modes are ready)
                else:
                    print(opcodes[command]['ABS'][2:], operand[3:], operand[1:3])