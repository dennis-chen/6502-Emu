from constants import opcodes
import argparse
import re
import os
import sys

# for negative hex values
def toHex(val, nbits):
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
    splitLine = l.split(' ')
    arguments = len(splitLine)
    return {'data':splitLine,'length':arguments}

def findAndReplaceLabels(lineDict,labels):
    #adds things to labels dictionary based on lineDict
    bytePosition = lineDict['bytePosition']
    arguments = lineDict['length']
    splitLine = lineDict['data']
    twoByteAddress = re.compile('\$[0-9]{4}')
    if (arguments == 3 and splitLine[0].endswith(':')):
        labels[splitLine[0][:-1]] = bytePosition
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
    if (arguments == 3 and splitLine[0].endswith(':')):
        if (twoByteAddress.search(splitLine[2])):
            return 3
        else:
            return 2
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
    else:
        print "Number arguments: " + str(arguments)
        raise ValueError("Number of arguments not between 1 and 3!")

def parseLines(f):
    #returns parsedLines and labels, where parsedLines is a 
    #list of dictionaries, each dictionary describing the contents
    #of the command on that line, and labels is the location
    #and names of jump labels
    bytePosition = 0
    parsedLines = []
    labels = {}
    for line in f:
        stripped = line.strip()
        if stripped: #stripped line is not empty
            print stripped
            lineDict = getLineDict(stripped)
            lineDict['bytePosition'] = bytePosition
            findAndReplaceLabels(lineDict,labels)
            parsedLines.append(lineDict)
            bytePosition += getByteSize(lineDict)
    return parsedLines, labels

def getHexFromLines(parsedLines,labels):
    hexOutput = ""
    for line in parsedLines:
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
                        raise ValueError('Problem with address in this line: ',line)
                else:
                    raise ValueError('Invalid line length')
            else:
                raise ValueError('No opcode found')
    return (hexOutput.strip())

def assemblyToHex(f):
    #takes file handler for text file  with assembly and returns string of hex vals
    parsedLines,labels = parseLines(f)
    hexString = getHexFromLines(parsedLines,labels)
    return hexString
    
if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-i","--input",type=argparse.FileType('r'))
    parser.add_argument("-o", "--output", help="Directs the output to a name of your choice")
    args = parser.parse_args()
    hexStr = assemblyToHex(args.input)
    with open(args.output, 'w') as output_file:
        output_file.write(hexStr)
