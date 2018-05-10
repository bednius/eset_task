
#include <iostream>
#include "InstructionSet.h"
#include "InstructionInfo.h"
#include "ArgumentType.h"


InstructionSet::InstructionSet() {
    for (int i = 0; i < 256; ++i)
        _instructions[i] = nullptr;

    _instructions[(uint8_t) InstructionType::mov] = new InstructionInfo(
            InstructionType::mov, 3,
            {ArgumentType::arg, ArgumentType::arg},
            &EvMachine::mov);

    _instructions[(uint8_t) InstructionType::loadConst] = new InstructionInfo(
            InstructionType::loadConst, 3,
            {ArgumentType::constant, ArgumentType::arg},
            &EvMachine::loadConst);

    _instructions[(uint8_t) InstructionType::add] = new InstructionInfo(
            InstructionType::add, 6,
            {ArgumentType::arg, ArgumentType::arg, ArgumentType::arg},
            &EvMachine::add);

    _instructions[(uint8_t) InstructionType::sub] = new InstructionInfo(
            InstructionType::sub, 6,
            {ArgumentType::arg, ArgumentType::arg, ArgumentType::arg},
            &EvMachine::sub);

    _instructions[(uint8_t) InstructionType::div] = new InstructionInfo(
            InstructionType::div, 6,
            {ArgumentType::arg, ArgumentType::arg, ArgumentType::arg},
            &EvMachine::div);

    _instructions[(uint8_t) InstructionType::mod] = new InstructionInfo(
            InstructionType::mod, 6,
            {ArgumentType::arg, ArgumentType::arg, ArgumentType::arg},
            &EvMachine::mod);

    _instructions[(uint8_t) InstructionType::mul] = new InstructionInfo(
            InstructionType::mul, 6,
            {ArgumentType::arg, ArgumentType::arg, ArgumentType::arg},
            &EvMachine::mul);

    _instructions[(uint8_t) InstructionType::compare] = new InstructionInfo(
            InstructionType::compare, 5,
            {ArgumentType::arg, ArgumentType::arg, ArgumentType::arg},
            &EvMachine::compare);

    _instructions[(uint8_t) InstructionType::jump] = new InstructionInfo(
            InstructionType::jump, 5,
            {ArgumentType::address},
            &EvMachine::jump);

    _instructions[(uint8_t) InstructionType::jumpEqual] = new InstructionInfo(
            InstructionType::jumpEqual, 5,
            {ArgumentType::address, ArgumentType::arg, ArgumentType::arg},
            &EvMachine::jumpEqual);

    _instructions[(uint8_t) InstructionType::read] = new InstructionInfo(
            InstructionType::read, 5,
            {ArgumentType::arg, ArgumentType::arg, ArgumentType::arg, ArgumentType::arg},
            &EvMachine::read);

    _instructions[(uint8_t) InstructionType::write] = new InstructionInfo(
            InstructionType::write, 5,
            {ArgumentType::arg, ArgumentType::arg, ArgumentType::arg},
            &EvMachine::write);

    _instructions[(uint8_t) InstructionType::consoleRead] = new InstructionInfo(
            InstructionType::consoleRead, 5,
            {ArgumentType::arg},
            &EvMachine::consoleRead);

    _instructions[(uint8_t) InstructionType::consoleWrite] = new InstructionInfo(
            InstructionType::consoleWrite, 5,
            {ArgumentType::arg},
            &EvMachine::consoleWrite);

    _instructions[(uint8_t) InstructionType::createThread] = new InstructionInfo(
            InstructionType::createThread, 5,
            {ArgumentType::address, ArgumentType::arg},
            &EvMachine::createThread);

    _instructions[(uint8_t) InstructionType::joinThread] = new InstructionInfo(
            InstructionType::joinThread, 5,
            {ArgumentType::arg},
            &EvMachine::joinThread);

    _instructions[(uint8_t) InstructionType::hlt] = new InstructionInfo(
            InstructionType::hlt, 5,
            {},
            &EvMachine::hlt);

    _instructions[(uint8_t) InstructionType::sleep] = new InstructionInfo(
            InstructionType::sleep, 5,
            {ArgumentType::arg},
            &EvMachine::sleep);

    _instructions[(uint8_t) InstructionType::call] = new InstructionInfo(
            InstructionType::call, 4,
            {ArgumentType::address},
            &EvMachine::call);

    _instructions[(uint8_t) InstructionType::ret] = new InstructionInfo(
            InstructionType::ret, 4,
            {},
            &EvMachine::ret);

    _instructions[(uint8_t) InstructionType::lock] = new InstructionInfo(
            InstructionType::lock, 4,
            {ArgumentType::arg},
            &EvMachine::lock);

    _instructions[(uint8_t) InstructionType::unlock] = new InstructionInfo(
            InstructionType::unlock, 4,
            {ArgumentType::arg},
            &EvMachine::unlock);
}


InstructionSet::~InstructionSet() {
    for (int i = 0; i < 256; ++i)
        delete _instructions[i];
}

InstructionInfo *InstructionSet::operator[](uint8_t index) {
    return _instructions[index];
}
