import re
import sys

# for negative hex values
def toHex(val, nbits):
    return hex((val + (1 << nbits)) % (1 << nbits))

def translate(fileName):
    opcodes = {
        '---': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'ADC': {'SNGL': 'null', 'INDY':   '71', 'IMM':   '69', 'INDX':   '61', 'ZPX':   '75', 'ABS':   '6d', 'ABSY':   '79', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP':   '65', 'ABSX':   '7d'},
        'AND': {'SNGL': 'null', 'INDY':   '31', 'IMM':   '29', 'INDX':   '21', 'ZPX':   '35', 'ABS':   '2d', 'ABSY':   '39', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP':   '25', 'ABSX':   '3d'},
        'ASL': {'SNGL':   '0a', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX':   '16', 'ABS':   '0e', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP':   '06', 'ABSX':   '1e'},
        'BCC': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA':   '90', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'BCS': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA':   'b0', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'BEQ': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA':   'f0', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'BIT': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS':   '2c', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP':   '24', 'ABSX': 'null'},
        'BMI': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA':   '30', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'BNE': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA':   'd0', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'BPL': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA':   '10', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'BRK': {'SNGL':   '00', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'BVC': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA':   '50', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'BVS': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA':   '70', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'CLC': {'SNGL':   '18', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'CLD': {'SNGL':   'd8', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'CLI': {'SNGL':   '58', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'CLV': {'SNGL':   'b8', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'CMP': {'SNGL': 'null', 'INDY':   'd1', 'IMM':   'c9', 'INDX':   'c1', 'ZPX':   'd5', 'ABS':   'cd', 'ABSY':   'd9', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP':   'c5', 'ABSX':   'dd'},
        'CPX': {'SNGL': 'null', 'INDY': 'null', 'IMM':   'e0', 'INDX': 'null', 'ZPX': 'null', 'ABS':   'ec', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP':   'e4', 'ABSX': 'null'},
        'CPY': {'SNGL': 'null', 'INDY': 'null', 'IMM':   'c0', 'INDX': 'null', 'ZPX': 'null', 'ABS':   'cc', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP':   'c4', 'ABSX': 'null'},
        'DEC': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX':   'd6', 'ABS':   'ce', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP':   'c6', 'ABSX':   'de'},
        'DEX': {'SNGL':   'ca', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'DEY': {'SNGL':   '88', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'EOR': {'SNGL': 'null', 'INDY':   '51', 'IMM':   '49', 'INDX':   '41', 'ZPX':   '55', 'ABS':   '4d', 'ABSY':   '59', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP':   '45', 'ABSX':   '5d'},
        'INC': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX':   'f6', 'ABS':   'ee', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP':   'e6', 'ABSX':   'fe'},
        'INX': {'SNGL':   'e8', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'INY': {'SNGL':   'c8', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'JMP': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS':   '4c', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND':   '6c', 'ZP': 'null', 'ABSX': 'null'},
        'JSR': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS':   '20', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'LDA': {'SNGL': 'null', 'INDY':   'b1', 'IMM':   'a9', 'INDX':   'a1', 'ZPX':   'b5', 'ABS':   'ad', 'ABSY':   'b9', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP':   'a5', 'ABSX':   'bd'},
        'LDX': {'SNGL': 'null', 'INDY': 'null', 'IMM':   'a2', 'INDX': 'null', 'ZPX': 'null', 'ABS':   'ae', 'ABSY':   'be', 'BRA': 'null', 'ZPY':   'b6', 'IND': 'null', 'ZP':   'a6', 'ABSX': 'null'},
        'LDY': {'SNGL': 'null', 'INDY': 'null', 'IMM':   'a0', 'INDX': 'null', 'ZPX':   'b4', 'ABS':   'ac', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP':   'a4', 'ABSX':   'bc'},
        'LSR': {'SNGL':   '4a', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX':   '56', 'ABS':   '4e', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP':   '46', 'ABSX':   '5e'},
        'NOP': {'SNGL':   'ea', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'ORA': {'SNGL': 'null', 'INDY':   '11', 'IMM':   '09', 'INDX':   '01', 'ZPX':   '15', 'ABS':   '0d', 'ABSY':   '19', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP':   '05', 'ABSX':   '1d'},
        'PHA': {'SNGL':   '48', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'PHP': {'SNGL':   '08', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'PLA': {'SNGL':   '68', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'PLP': {'SNGL':   '28', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'ROL': {'SNGL':   '2a', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX':   '36', 'ABS':   '2e', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP':   '26', 'ABSX':   '3e'},
        'ROR': {'SNGL':   '6a', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX':   '76', 'ABS':   '6e', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP':   '66', 'ABSX':   '7e'},
        'RTI': {'SNGL':   '40', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'RTS': {'SNGL':   '60', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'SBC': {'SNGL': 'null', 'INDY':   'f1', 'IMM':   'e9', 'INDX':   'e1', 'ZPX':   'f5', 'ABS':   'ed', 'ABSY':   'f9', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP':   'e5', 'ABSX':   'fd'},
        'SEC': {'SNGL':   '38', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'SED': {'SNGL':   'f8', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'SEI': {'SNGL':   '78', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'STA': {'SNGL': 'null', 'INDY':   '91', 'IMM': 'null', 'INDX':   '81', 'ZPX':   '95', 'ABS':   '8d', 'ABSY':   '99', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP':   '85', 'ABSX':   '9d'},
        'STX': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS':   '8e', 'ABSY': 'null', 'BRA': 'null', 'ZPY':   '96', 'IND': 'null', 'ZP':   '86', 'ABSX': 'null'},
        'STY': {'SNGL': 'null', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX':   '94', 'ABS':   '8c', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP':   '84', 'ABSX': 'null'},
        'TAX': {'SNGL':   'aa', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'TAY': {'SNGL':   'a8', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'TSX': {'SNGL':   'ba', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'TXA': {'SNGL':   '8a', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'TXS': {'SNGL':   '9a', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'TYA': {'SNGL':   '98', 'INDY': 'null', 'IMM': 'null', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP': 'null', 'ABSX': 'null'},
        'WDM': {'SNGL': 'null', 'INDY': 'null', 'IMM':   '42', 'INDX': 'null', 'ZPX': 'null', 'ABS': 'null', 'ABSY': 'null', 'BRA': 'null', 'ZPY': 'null', 'IND': 'null', 'ZP':   '42', 'ABSX': 'null'},
    }

    twoByteAddress = re.compile('\$[0-9]{4}')
    file = open(fileName, 'r')

    bytePosition = 0
    processedLines = []
    labels = {}

    # loop to get positions of labels 
    for line in file:
        # strip comments and newlines
        commentStart = line.find(';')
        if (commentStart != -1):
            line = line[0:commentStart]
        line = line.rstrip()
        splitLine = line.split(' ')
        arguments = len(splitLine)
        processedLines.append({'data': splitLine, 'length': arguments, 'bytePosition': bytePosition})

        # we have a label AND an opcode AND an address on this line
        if (arguments == 3 and splitLine[0].endswith(':')):
            # remove colon before adding it to labels
            labels[splitLine[0][:-1]] = bytePosition
            if (twoByteAddress.search(splitLine[2])):
                bytePosition += 3
            else:
                bytePosition += 2

        elif (arguments == 2):
            # label and implicitly addressed opcode
            if (splitLine[0].endswith(':')):
                # remove colon before adding it to labels
                labels[splitLine[0][:-1]] = bytePosition
                bytePosition += 1
            # opcode and address
            else:
                if (twoByteAddress.search(splitLine[1])):
                    bytePosition += 3
                else:
                    bytePosition += 2

        elif (arguments == 1):
            # just a label
            if (splitLine[0].endswith(':')):
                # remove colon before adding it to labels
                labels[splitLine[0][:-1]] = bytePosition
            # just an implicitly addressed opcode
            else:
                bytePosition += 1

    bytePosition = 0
    hexOutput = ""

    # loop to actually convert
    for line in processedLines:
        # take the labels out of lines
        if (line['data'][0].endswith(':')):
            line['length'] -= 1
            line['data'] = line['data'][1:]

        # only process lines that aren't just labels
        if(line['length'] > 0):
            opcode = line['data'][0]
            address = 'null'
            if (line['length']) == 2:
                address = line['data'][1]

            if (opcode in opcodes.keys()):
                # implicit
                if (line['length'] == 1):
                    hexOutput += (opcodes[opcode]['SNGL']) + ' '
                elif (line['length'] == 2):
                    # immediate
                    if(address.startswith('#')):
                        hexOutput += opcodes[opcode]['IMM'] + ' ' + address[2:] + ' '
                    elif (address.startswith('(')):
                        # indexed indirect
                        if address[4] == ',':
                            hexOutput += opcodes[opcode]['INDX'] + ' ' + address[2:4] + ' '
                        # indirect index
                        elif address[5] == ',':
                            hexOutput += opcodes[opcode]['INDY'] + ' ' + address[2:4] + ' '
                        # indirect
                        else:
                            hexOutput += opcodes[opcode]['IND'] + ' ' + address[4:6] + ' ' + address[2:4] + ' '
                    elif (address.startswith('$')):
                        operandLength = len(address)
                        # absolute X and Y
                        if (operandLength == 7):
                            mode = 'ABS' + address[6].upper()
                            hexOutput += opcodes[opcode][mode] + ' ' + address[3:5] + ' ' + address[1:3] + ' '
                        # zero page
                        elif (operandLength == 3):
                            hexOutput += opcodes[opcode]['ZP'] + ' ' + address[1:] + ' '
                        # zero page X and Y
                        elif (address[3] == ','):
                            mode = 'ZP' + address[4].upper()
                            hexOutput += opcodes[opcode][mode] + ' ' + address[1:3] + ' '
                        # absolute
                        else:
                            hexOutput += opcodes[opcode]['ABS'] + ' ' + address[3:] + ' ' + address[1:3] + ' '
                    elif (address in labels.keys()):
                        # relative addressing with label
                        byteDiff = labels[address] - line['bytePosition'] - 2
                        hexOutput += opcodes[opcode]['BRA'] + ' ' + toHex(byteDiff, 8)[2:] + ' '
                    else:
                        break
                else:
                    break
            else:
                break

    return (hexOutput.strip())
    
if __name__ == "__main__":
    if len(sys.argv) == 2:
        sys.stdout.write(translate(sys.argv[1]))