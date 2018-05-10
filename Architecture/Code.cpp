
#include <iostream>
#include "Code.h"


Code::Code(uint8_t *data, int64_t size) {
    _code_set = boost::dynamic_bitset<>(size * 8);
    for (int64_t i = 0; i < size; ++i) {
        uint8_t c = (uint8_t) data[i];
        for (int j = 7; j > -1; --j) {
            _code_set[i * 8 + j] = c & 1;
            c = c >> 1;
        }
    }
}


Code::~Code() {
    _code_set.clear();
}


InstructionInfo *Code::getInstruction(uint64_t &instruction_pointer) {
    if (instruction_pointer >= _code_set.size())
        return nullptr;
    if (_code_set.size() - instruction_pointer <= 7) {
        return tryGetInstruction(instruction_pointer);
    }

    uint8_t key = 0;
    InstructionInfo *result = nullptr;

    key |= (uint8_t(_code_set[instruction_pointer++]) << 7);
    key |= (uint8_t(_code_set[instruction_pointer++]) << 6);

    for (int i = 5; i >= 2 and result == nullptr; --i) {
        key |= (uint8_t(_code_set[instruction_pointer++]) << i);
        result = _instruction_set[key];
        if (result != nullptr and result->_key_length == 8 - i) // 4 5 6
            result = _instruction_set[key];
        else
            result = nullptr;
    }

    return result;
}

/*
 * tries get instruction from last byte
 * it can be only hlt or ret
 */
InstructionInfo *Code::tryGetInstruction(uint64_t &instruction_ptr) {
    uint64_t localPointer = instruction_ptr;
    uint64_t leftBits = _code_set.size() - localPointer;
    uint8_t key = 0;

    if(leftBits >= 4) {
        for (int i = 7; i >= 3; --i)
            key |= uint8_t(_code_set[localPointer++]) << i;
        InstructionInfo *info = _instruction_set[key];
        if(info) {
            if (info->_instruction_type == InstructionType::ret) {
                instruction_ptr = localPointer;
                return info;
            }
            if (leftBits > 4 and info->_instruction_type == InstructionType::hlt) {
                instruction_ptr = ++localPointer;
                return info;
            }
        }
    }
    return nullptr;
}


uint8_t Code::getArg(uint64_t &instruction_pointer) {
    uint8_t result = 0;
    result |= (_code_set[instruction_pointer++] << 6);
    if(result == 64) {
        for (int i = 4; i < 6; ++i)
            result |= (_code_set[instruction_pointer++] << i);
    }
    for (int i = 0; i < 4; ++i)
        result |= (_code_set[instruction_pointer++] << i);
    return result;
}


uint32_t Code::getCodeAddr(uint64_t &instruction_pointer) {
    uint32_t result = (uint32_t) getBytesLe(instruction_pointer, 4);
    return result;
}

/*
 * read bytes in little endian bitwise
 */
uint64_t Code::getBytesLe(uint64_t &instruction_ptr, uint8_t bytes_count) {
    uint64_t result = 0;
    for (int i = 0; i < bytes_count * 8; ++i) {
        result += (uint64_t(_code_set[instruction_ptr++]) << i);
    }
    return result;
}


int64_t Code::getConstant(uint64_t &instruction_pointer) {
    int64_t result = (int64_t) getBytesLe(instruction_pointer, 8);
    return result;
}


uint64_t Code::getSize() {
    return _code_set.size();
}


