
#ifndef ESET_TEST_INSTRUCTION_TYPE_H
#define ESET_TEST_INSTRUCTION_TYPE_H

enum class InstructionType : uint8_t {
    mov = 0,                //000 00000
    loadConst = 32,         //001 00000
    add = 68,
    sub = 72,
    div = 76,
    mod = 80,
    mul = 84,
    compare = 96,
    jump = 104,
    jumpEqual = 112,
    read = 128,
    write = 136,
    consoleRead = 144,
    consoleWrite = 152,
    createThread = 160,
    joinThread = 168,
    hlt = 176,
    sleep = 184,
    call = 192,
    ret = 208,
    lock = 224,
    unlock = 240
};


#endif //ESET_TEST_INSTRUCTION_TYPE_H
