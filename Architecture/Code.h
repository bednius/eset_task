
#ifndef ESET_TEST_CODE_H
#define ESET_TEST_CODE_H

#include <boost/dynamic_bitset.hpp>
#include <ostream>
#include "InstructionSet.h"

class Code {
public:
    explicit Code(uint8_t *data, int64_t size);

    virtual ~Code();

    InstructionInfo *getInstruction(uint64_t &instruction_pointer);

    uint8_t getArg(uint64_t &instruction_pointer);

    uint32_t getCodeAddr(uint64_t &instruction_pointer);

    int64_t getConstant(uint64_t &instruction_pointer);

    uint64_t getSize();

private:
    boost::dynamic_bitset<> _code_set;

    InstructionSet _instruction_set;

    InstructionInfo *tryGetInstruction(uint64_t &instruction_ptr);

    uint64_t getBytesLe(uint64_t &instruction_ptr, uint8_t bytes_count);
};


#endif //ESET_TEST_MEMORY_H
