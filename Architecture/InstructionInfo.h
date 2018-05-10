
#ifndef ESET_TEST_INSTRUCTION_INFO_H
#define ESET_TEST_INSTRUCTION_INFO_H

#include <ostream>
#include "InstructionType.h"
#include "ArgumentType.h"
#include "EvMachine.h"

class InstructionInfo {
public:
    void (EvMachine::*_invoke)(EvThread *) = nullptr;

    InstructionInfo(InstructionType instruction_type, uint8_t key_length,
                    std::initializer_list<ArgumentType> arg_types,
                    void (EvMachine::*invoke)(EvThread *));

    virtual ~InstructionInfo();

private:
    InstructionType _instruction_type;

    uint8_t _args_num;

    ArgumentType *_arg_types;

    uint8_t _key_length;

    friend class EvThread;

    friend class Code;
};


#endif //ESET_TEST_INSTRUCTION_INFO_H
