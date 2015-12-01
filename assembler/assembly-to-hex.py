from constants import opcodes
import argparse
import re
import os
import sys

# for negative hex values
def toHex(val, nbits):
    print("hexvalue", hex((val + (1 << nbits)) % (1 << nbits)))
    return hex((val + (1 << nbits)) % (1 << nbits))

def stripCommentsAndWhitespace(line):
    # strip comments and whitespace
    commentStart = line.find(';')
    if (commentStart != -1):
        return line[0:commentStart].rstrip() 
    else:
        return line.rstrip()

def getLineDict(line):
    #returns dict of data and length for a line
    l = stripCommentsAndWhitespace(line)
    splitLine = filter(None, l.split(' '))
    #print(splitLine)
    arguments = len(splitLine)
    #print("Arguments", splitLine)
    return {'data':splitLine,'length':arguments}

def findLabelsAndVariables(lineDict,labels,variables):
    #adds things to labels dictionary based on lineDict
    bytePosition = lineDict['bytePosition']
    arguments = lineDict['length']
    splitLine = lineDict['data']
    twoByteAddress = re.compile('\$[0-9]{4}')
    if (arguments == 3):
        if (splitLine[0].endswith(':')):
            labels[splitLine[0][:-1]] = bytePosition
        elif (splitLine[0] == 'define'):
            variables[splitLine[1]] = splitLine[2];
    elif (arguments == 2):
        # label and implicitly addressed opcode
        if (splitLine[0].endswith(':')):
            # remove colon before adding it to labels
            labels[splitLine[0][:-1]] = bytePosition
    elif (arguments == 1):
        # just a label
        if (splitLine[0].endswith(':')):
            # remove colon before adding it to labels
            labels[splitLine[0][:-1]] = bytePosition

def getByteSize(lineDict):
    #specifies size of line in terms of bytes once
    #compiled to hex
    bytePosition = lineDict['bytePosition']
    arguments = lineDict['length']
    splitLine = lineDict['data']
    twoByteAddress = re.compile('\$[0-9]{4}')
    # label declaration
    if (arguments == 3):
        if (splitLine[0].endswith(':')):
            if (twoByteAddress.search(splitLine[2])):
                return 3
            else:
                return 2
        else:
            # variable declaration
            return 0
    elif (arguments == 2):
        # label and implicitly addressed opcode
        if (splitLine[0].endswith(':')):
            # remove colon before adding it to labels
            return 1
        # opcode and address
        else:
            if (twoByteAddress.search(splitLine[1])):
                return 3
            else:
                return 2
    elif (arguments == 1):
        # just a label
        if (splitLine[0].endswith(':')):
            # remove colon before adding it to labels
            return 0
        # just an implicitly addressed opcode
        else:
            return 1
    elif (arguments == 0):
        return 0
    else:
        print("Number arguments: " + str(arguments))
        raise ValueError("Number of arguments not between 0 and 3!")

def parseLines(f):
    #returns parsedLines and labels, where parsedLines is a 
    #list of dictionaries, each dictionary describing the contents
    #of the command on that line, and labels is the location
    #and names of jump labels
    bytePosition = 0
    parsedLines = []
    labels = {}
    variables = {}
    for line in f:
        stripped = line.strip()
        if stripped: #stripped line is not empty
            #print(stripped)
            lineDict = getLineDict(stripped)
            lineDict['bytePosition'] = bytePosition
            findLabelsAndVariables(lineDict,labels,variables)
            parsedLines.append(lineDict)
            bytePosition += getByteSize(lineDict)
    return parsedLines, labels, variables

def padHex(s):
    if (len(s) == 1):
        return "0" + s
    else:
        return s

def getHexFromLines(parsedLines,labels,variables):
    print("Labels:", labels)
    print("Variables:", variables)
    hexOutput = ""
    for line in parsedLines:
        if(line['length'] > 0):
            # take the labels out of lines
            if (line['data'][0].endswith(':')):
                line['length'] -= 1
                line['data'] = line['data'][1:]
            # only process lines that aren't just labels or variables
            if(line['length'] > 0 and line['data'][0].lower() != 'define'):
                opcode = line['data'][0].upper()
                address = 'null'
                if (line['length']) == 2:
                    address = line['data'][1]
                    # replace variables with their values
                    for variable in variables.keys():
                        if variable in address:
                            address = address.replace(variable, variables[variable])
                print("Opcode", opcode, "Address", address)
                if (opcode in opcodes.keys()):
                    # implicit
                    if (line['length'] == 1):
                        print((opcodes[opcode]['SNGL']) + ' ')
                        hexOutput += (opcodes[opcode]['SNGL']) + ' '
                    elif (line['length'] == 2):
                        # immediate
                        if(address.startswith('#')):
                            if(address[1] == '$'):
                                print(opcodes[opcode]['IMM'] + ' ' + padHex(address[2:]) + ' ')
                                hexOutput += opcodes[opcode]['IMM'] + ' ' + padHex(address[2:]) + ' '
                            else:
                                print(opcodes[opcode]['IMM'] + ' ' + padHex(address[1:]) + ' ')
                                hexOutput += opcodes[opcode]['IMM'] + ' ' + padHex(address[1:]) + ' '
                        elif (address.startswith('(')):
                            # indexed indirect
                            if address[4] == ',':
                                print(opcodes[opcode]['INDX'] + ' ' + address[2:4] + ' ')
                                hexOutput += opcodes[opcode]['INDX'] + ' ' + address[2:4] + ' '
                            # indirect index
                            elif address[5] == ',':
                                print(opcodes[opcode]['INDY'] + ' ' + address[2:4] + ' ')
                                hexOutput += opcodes[opcode]['INDY'] + ' ' + address[2:4] + ' '
                            # indirect
                            else:
                                print(opcodes[opcode]['IND'] + ' ' + address[4:6] + ' ' + address[2:4] + ' ')
                                hexOutput += opcodes[opcode]['IND'] + ' ' + address[4:6] + ' ' + address[2:4] + ' '
                        elif (address.startswith('$')):
                            operandLength = len(address)
                            # absolute X and Y
                            if (operandLength == 7):
                                mode = 'ABS' + address[6].upper()
                                print(opcodes[opcode][mode] + ' ' + address[3:5] + ' ' + address[1:3] + ' ')
                                hexOutput += opcodes[opcode][mode] + ' ' + address[3:5] + ' ' + address[1:3] + ' '
                            # zero page
                            elif (operandLength == 3):
                                print(opcodes[opcode]['ZP'] + ' ' + padHex(address[1:]) + ' ')
                                hexOutput += opcodes[opcode]['ZP'] + ' ' + padHex(address[1:]) + ' '
                            # zero page X and Y
                            elif (address[3] == ','):
                                mode = 'ZP' + address[4].upper()
                                print(opcodes[opcode][mode] + ' ' + address[1:3] + ' ')
                                hexOutput += opcodes[opcode][mode] + ' ' + address[1:3] + ' '
                            # absolute
                            else:
                                print(opcodes[opcode]['ABS'] + ' ' + padHex(address[3:]) + ' ' + address[1:3] + ' ')
                                hexOutput += opcodes[opcode]['ABS'] + ' ' + padHex(address[3:]) + ' ' + address[1:3] + ' '
                        elif (address in labels.keys()):
                            # print ("we\'ve got a label")
                            if (opcode[0] == "J"):
                                # Jump to an absolute point (this doesn't account for indirect jumps)
                                byteDiff = labels[address] - line['bytePosition'] - 2
                                print(opcodes[opcode]['ABS'] + ' ' + padHex(str(toHex(byteDiff, 16)[2:])) + ' 06 ' )
                                hexOutput += opcodes[opcode]['ABS'] + ' ' + padHex(str(toHex(byteDiff, 16)[2:])) + ' 06 ' 
                            else:
                            # relative addressing with label
                                byteDiff = labels[address] - line['bytePosition'] - 2
                                print(opcodes[opcode]['BRA'] + ' ' + padHex(str(toHex(byteDiff, 8)[2:])) + ' ')
                                hexOutput += opcodes[opcode]['BRA'] + ' ' + padHex(str(toHex(byteDiff, 8)[2:])) + ' '
                        else:
                            raise ValueError('Problem with address in this line: ',line)
                    else:
                        raise ValueError('Invalid line length')
                else:
                    raise ValueError('No opcode found')
    return (hexOutput.strip())

def assemblyToHex(f):
    #takes file handler for text file  with assembly and returns string of hex vals
    parsedLines,labels,variables = parseLines(f)
    hexString = getHexFromLines(parsedLines,labels,variables)
    return hexString
    
if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-i","--input",type=argparse.FileType('r'))
    parser.add_argument("-o", "--output", help="Directs the output to a name of your choice")
    args = parser.parse_args()
    hexStr = assemblyToHex(args.input)
    with open(args.output, 'w') as output_file:
        output_file.write(hexStr)
