
#include "InstructionInfo.h"


InstructionInfo::InstructionInfo(InstructionType instruction_type,
                                 uint8_t key_length,
                                 std::initializer_list<ArgumentType> arg_types,
                                 void (EvMachine::*invoke)(EvThread *)) :
        _instruction_type(instruction_type), _key_length(key_length), _invoke(invoke) {

    _args_num = arg_types.size();

    if (_args_num > 0) {
        _arg_types = new ArgumentType[_args_num];
        auto it = arg_types.begin();
        for (int i = 0; i < _args_num; ++i) {
            _arg_types[i] = *it++;
        }
    }
}


InstructionInfo::~InstructionInfo() {
    delete[] _arg_types;
}


