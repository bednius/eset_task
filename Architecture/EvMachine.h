
#ifndef ESET_TEST_EV_MACHINE_H
#define ESET_TEST_EV_MACHINE_H

#include <mutex>
#include <thread>
#include <map>
#include <boost/dynamic_bitset.hpp>
#include "EvMemory.h"
#include "EvThread.h"
#include "ThreadsManager.h"

class Code;

class EvMachine {
public:
    explicit EvMachine(std::string const &filename);

    virtual ~EvMachine();

    void start();

    uint64_t readVal(uint8_t arg, int64_t *registers);

    void writeVal(uint8_t arg, int64_t *registers, uint64_t val);

    Code *getCode();

    void mov(EvThread *);

    void loadConst(EvThread *);

    void add(EvThread *);

    void sub(EvThread *);

    void div(EvThread *);

    void mod(EvThread *);

    void mul(EvThread *);

    void compare(EvThread *);

    void jump(EvThread *);

    void jumpEqual(EvThread *);

    void read(EvThread *);

    void write(EvThread *);

    void consoleRead(EvThread *);

    void consoleWrite(EvThread *);

    void createThread(EvThread *);

    void joinThread(EvThread *);

    void hlt(EvThread *);

    void sleep(EvThread *);

    void call(EvThread *);

    void ret(EvThread *);

    void lock(EvThread *);

    void unlock(EvThread *);

private:
    Code *_code = nullptr;

    ThreadsManager *_threads_manager;

    EvMemory *_ev_memory = nullptr;

    std::string _program_name;

    std::map<uint64_t, std::mutex> _locks;

    std::ifstream *_input = nullptr;

    std::ofstream *_output = nullptr;

    std::mutex _locks_mtx;

    std::mutex _ifstream_mtx;

    std::mutex _ofstream_mtx;
};


#endif //ESET_TEST_EV_MACHINE_H
