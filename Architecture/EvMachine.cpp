
#include <iostream>
#include <zconf.h>
#include <cmath>
#include <random>
#include <fstream>
#include <iomanip>
#include "EvMachine.h"
#include "Code.h"


uint64_t EvMachine::readVal(uint8_t arg, int64_t *registers) {
    uint8_t reg_index = arg & 0x0F;
    uint64_t reg_val = registers[reg_index];
    bool is_mem = arg >> 6;

    if (is_mem) {
        uint8_t bytes_num = pow(2, ((arg >> 4) & 0x03));
        return _ev_memory->read_bytes(reg_val, bytes_num);
    }
    return reg_val;
}


void EvMachine::writeVal(uint8_t arg, int64_t *registers, uint64_t val) {
    uint8_t reg_index = arg & 0x0F;
    bool is_mem = arg >> 6;

    if (is_mem) {
        uint8_t bytes_num = pow(2, ((arg >> 4) & 0x03));
        uint64_t reg_val = registers[reg_index];
        _ev_memory->write_bytes(reg_val, bytes_num, val);
    } else {
        registers[reg_index] = val;
    }
}


void EvMachine::start() {
    _threads_manager->spawnMainThread();
}


EvMachine::EvMachine(std::string const &filename) {
    _program_name = filename.substr(0, filename.find(".evm"));

    _threads_manager = new ThreadsManager(this);

    std::ifstream in(filename, std::ios::in | std::ios::binary);
    std::string buffer;
    if (in) {
        in.seekg(0, std::ios::end);
        buffer.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&buffer[0], buffer.size());
        in.close();
    } else {
        std::cerr << "Cannot open file" << std::endl;
        exit(EXIT_FAILURE);
    }

    uint32_t header[3];     // header[0 - codeSize, 1-dataSize, 2-initData]
    for (int i = 0; i < 3; ++i) {
        header[i] = 0;
        for (int j = 0; j < 4; ++j) {
            header[i] |= ((uint8_t) buffer[8 + i * 4 + j] << (j * 8));
        }
    }

    _code = new Code((uint8_t *) &buffer[20], header[0]);

    _ev_memory = new EvMemory(header[1]);
    _ev_memory->fill_mem((uint8_t *) &buffer[20 + header[0]], header[2]);
    buffer.clear();
}


EvMachine::~EvMachine() {
    if (_input) {
        _input->close();
        delete _input;
    }
    if (_output) {
        _output->close();
        delete _output;
    }
    delete _threads_manager;
    delete _code;
    delete _ev_memory;
}


Code *EvMachine::getCode() {
    return _code;
}


void EvMachine::mov(EvThread *ev_thread) {
    uint64_t value = readVal(ev_thread->_args[0], ev_thread->_registers);
    writeVal(ev_thread->_args[1], ev_thread->_registers, value);
}


void EvMachine::loadConst(EvThread *ev_thread) {
    writeVal(ev_thread->_args[1], ev_thread->_registers, ev_thread->_args[0]);
}


void EvMachine::add(EvThread *ev_thread) {
    int64_t arg1_val = readVal(ev_thread->_args[0], ev_thread->_registers);
    int64_t arg2_val = readVal(ev_thread->_args[1], ev_thread->_registers);
    writeVal(ev_thread->_args[2], ev_thread->_registers, arg1_val + arg2_val);
}


void EvMachine::sub(EvThread *ev_thread) {
    int64_t arg1_val = readVal(ev_thread->_args[0], ev_thread->_registers);
    int64_t arg2_val = readVal(ev_thread->_args[1], ev_thread->_registers);
    writeVal(ev_thread->_args[2], ev_thread->_registers, arg1_val - arg2_val);
}


void EvMachine::div(EvThread *ev_thread) {
    int64_t arg1_val = readVal(ev_thread->_args[0], ev_thread->_registers);
    int64_t arg2_val = readVal(ev_thread->_args[1], ev_thread->_registers);
    if (arg2_val == 0) {
        std::cerr << "Divide by 0" << std::endl;
        exit(EXIT_FAILURE);
    }
    writeVal(ev_thread->_args[2], ev_thread->_registers, arg1_val / arg2_val);
}


void EvMachine::mod(EvThread *ev_thread) {
    int64_t arg1_val = readVal(ev_thread->_args[0], ev_thread->_registers);
    int64_t arg2_val = readVal(ev_thread->_args[1], ev_thread->_registers);
    writeVal(ev_thread->_args[2], ev_thread->_registers, arg1_val % arg2_val);
}


void EvMachine::mul(EvThread *ev_thread) {
    int64_t arg1_val = readVal(ev_thread->_args[0], ev_thread->_registers);
    int64_t arg2_val = readVal(ev_thread->_args[1], ev_thread->_registers);
    writeVal(ev_thread->_args[2], ev_thread->_registers, arg1_val * arg2_val);
}


void EvMachine::compare(EvThread *ev_thread) {
    int64_t val = 0;
    int64_t arg1_val = readVal(ev_thread->_args[0], ev_thread->_registers);
    int64_t arg2_val = readVal(ev_thread->_args[1], ev_thread->_registers);
    if (arg1_val < arg2_val)
        val = -1;
    else if (arg1_val > arg2_val)
        val = 1;
    writeVal(ev_thread->_args[2], ev_thread->_registers, val);
}


void EvMachine::jump(EvThread *ev_thread) {
    ev_thread->_instruction_ptr = ev_thread->_args[0];
}


void EvMachine::jumpEqual(EvThread *ev_thread) {
    int64_t arg1_val = readVal(ev_thread->_args[1], ev_thread->_registers);
    int64_t arg2_val = readVal(ev_thread->_args[2], ev_thread->_registers);

    if (arg1_val == arg2_val)
        ev_thread->_instruction_ptr = ev_thread->_args[0];
}


void EvMachine::consoleRead(EvThread *ev_thread) {
    uint64_t val;
    std::cin >> std::hex >> val;
    writeVal(ev_thread->_args[0], ev_thread->_registers, val);
}


void EvMachine::consoleWrite(EvThread *ev_thread) {
    int64_t arg1_val = readVal(ev_thread->_args[0], ev_thread->_registers);
    std::cout << std::hex << std::setfill('0') << std::setw(16) << arg1_val << std::endl;
}


void EvMachine::sleep(EvThread *ev_thread) {
    uint64_t arg1_val = readVal(ev_thread->_args[0], ev_thread->_registers);
    std::this_thread::sleep_for(std::chrono::milliseconds(arg1_val));
}


void EvMachine::call(EvThread *ev_thread) {
    ev_thread->_call_stack.push(ev_thread->_instruction_ptr);
    ev_thread->_instruction_ptr = ev_thread->_args[0];
}


void EvMachine::ret(EvThread *ev_thread) {
    ev_thread->_instruction_ptr = ev_thread->_call_stack.top();
    ev_thread->_call_stack.pop();
}


void EvMachine::lock(EvThread *ev_thread) {
    uint64_t arg1_val = readVal(ev_thread->_args[0], ev_thread->_registers);
    _locks_mtx.lock();
    std::mutex &lock = _locks[arg1_val];
    _locks_mtx.unlock();
    lock.lock();
}


void EvMachine::unlock(EvThread *ev_thread) {
    uint64_t arg1_val = readVal(ev_thread->_args[0], ev_thread->_registers);
    _locks_mtx.lock();
    _locks[arg1_val].unlock();
    _locks_mtx.unlock();
}


void EvMachine::read(EvThread *ev_thread) {
    _ifstream_mtx.lock();
    if (!_input) {
        _input = new std::ifstream(_program_name + ".in", std::ios::in | std::ios::binary);
        if (!_input) {
            std::cerr << "Cannot open file " + _program_name + ".in" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    uint64_t offset = readVal(ev_thread->_args[0], ev_thread->_registers);
    uint64_t bytes_to_read = readVal(ev_thread->_args[1], ev_thread->_registers);
    uint64_t mem_address = readVal(ev_thread->_args[2], ev_thread->_registers);

    char *buffer = new char[bytes_to_read];

    _input->seekg(offset, std::ios::beg);
    _input->read(buffer, bytes_to_read);

    _ev_memory->write_bytes(mem_address, _input->gcount(), (uint8_t *) buffer);

    writeVal(ev_thread->_args[3], ev_thread->_registers, _input->gcount());

    _input->seekg(0, std::ios::beg);
    delete[] buffer;
    _ifstream_mtx.unlock();
}


void EvMachine::write(EvThread *ev_thread) {
    _ofstream_mtx.lock();
    if (!_output) {
        _output = new std::ofstream(_program_name + ".out", std::ios::out | std::ios::binary | std::ios::trunc);
        if (!_output) {
            std::cerr << "Cannot open file " + _program_name + ".out" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    _output->seekp(0, std::ios::beg);

    uint64_t offset = readVal(ev_thread->_args[0], ev_thread->_registers);
    uint64_t bytes_num = readVal(ev_thread->_args[1], ev_thread->_registers);
    uint64_t mem_address = readVal(ev_thread->_args[2], ev_thread->_registers);

    char *buffer = new char[bytes_num];
    _ev_memory->read_bytes(mem_address, bytes_num, (uint8_t *) buffer);
    _output->seekp(offset, std::ios::beg);
    _output->write(buffer, bytes_num);
    _output->seekp(0, std::ios::beg);
    delete[] buffer;
    _ofstream_mtx.unlock();
}


void EvMachine::createThread(EvThread *ev_thread) {
    uint64_t evm_tid = _threads_manager->spawnThread(ev_thread->_args[0], ev_thread->_registers);
    writeVal(ev_thread->_args[1], ev_thread->_registers, evm_tid);
}


void EvMachine::joinThread(EvThread *ev_thread) {
    uint64_t arg1_val = readVal(ev_thread->_args[0], ev_thread->_registers);
    _threads_manager->joinThread(arg1_val);
}


void EvMachine::hlt(EvThread *ev_thread) {
    ev_thread->_instruction_ptr = _code->getSize();
}



