
#include <algorithm>
#include <iostream>
#include "EvMemory.h"


EvMemory::EvMemory(uint32_t pMemSize) : _size(pMemSize) {
    _data = new uint8_t[pMemSize];
    for (int i = 0; i < pMemSize; ++i)
        _data[i] = 0;
}


EvMemory::~EvMemory() {
    delete[] _data;
}


void EvMemory::fill_mem(uint8_t *data, uint32_t bytes_num) {
    if (bytes_num > _size) {
        std::cerr << "Initialized memory is too small" << std::endl;
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < bytes_num; ++i) {
        _data[i] = data[i];
    }
}


uint64_t EvMemory::read_bytes(uint32_t address, uint32_t bytes_num) {
    if (_size - bytes_num < address) {
        std::cerr << "Memory out of bound" << std::endl;
        exit(EXIT_FAILURE);
    }
    uint64_t result = 0;
    for (uint64_t i = bytes_num; i != 0; --i) {
        uint64_t shift = i - 1;
        uint64_t tmp = 0;
        tmp = _data[address + shift];
        tmp <<= (shift * 8);
        result += tmp;
    }
    return result;
}


void EvMemory::write_bytes(uint32_t address, uint32_t bytes_num, uint64_t val) {
    if (_size - bytes_num < address) {
        std::cerr << "Memory out of bound" << std::endl;
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < bytes_num; ++i) {
        uint8_t byte_val = (val >> (i * 8)) & 0xff;
        _data[address + i] = byte_val;
    }
}


void EvMemory::read_bytes(uint32_t address, uint32_t bytes_num, uint8_t *buff) {
    if (_size - bytes_num < address) {
        std::cerr << "Memory out of bound" << std::endl;
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < bytes_num; ++i) {
        buff[i] = _data[address + i];
    }
}


void EvMemory::write_bytes(uint32_t address, uint32_t bytes_num, uint8_t *buff) {
    if (_size - bytes_num < address) {
        std::cerr << "Memory out of bound" << std::endl;
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < bytes_num; ++i) {
        _data[address + i] = buff[i];
    }
}

