
#ifndef ESET_TEST_INSTRUCTION_SET_H
#define ESET_TEST_INSTRUCTION_SET_H

#include <ostream>
#include "InstructionInfo.h"

class InstructionSet {
public:
    InstructionSet();

    virtual ~InstructionSet();

    InstructionInfo *operator[](uint8_t index);

private:
    InstructionInfo *_instructions[256];
};


#endif //ESET_TEST_INSTRUCTION_SET_H
