
#include <iostream>
#include <zconf.h>
#include <cstring>
#include "EvThread.h"
#include "InstructionInfo.h"
#include "Code.h"


EvThread::EvThread(EvMachine *ev_machine) : _ev_machine(ev_machine) {
    for (int i = 0; i < 16; ++i)
        _registers[i] = 0;
    for (int i = 0; i < 4; ++i)
        _args[i] = 0;
    _code = ev_machine->getCode();
}


EvThread::EvThread(EvMachine *ev_machine, const int64_t *registers, uint32_t instruction_ptr, uint64_t ev_tid)
        : _ev_machine(ev_machine), _instruction_ptr(instruction_ptr), _ev_tid(ev_tid) {
    for (int i = 0; i < 16; ++i)
        this->_registers[i] = registers[i];
    for (int i = 0; i < 4; ++i)
        _args[i] = 0;
    _code = ev_machine->getCode();
}


uint64_t EvThread::getTid() const {
    return _ev_tid;
}


void EvThread::run() {
    InstructionInfo *instruction;
    while ((instruction = _code->getInstruction(_instruction_ptr))) {
        for (int i = 0; i < instruction->_args_num; ++i) {
            switch (instruction->_arg_types[i]) {
                case ArgumentType::arg :
                    _args[i] = _code->getArg(_instruction_ptr);
                    break;
                case ArgumentType::address :
                    _args[i] = _code->getCodeAddr(_instruction_ptr);
                    break;
                case ArgumentType::constant :
                    _args[i] = _code->getConstant(_instruction_ptr);
                    break;
            }
        }
        (_ev_machine->*(instruction->_invoke))(this);
    }
}
