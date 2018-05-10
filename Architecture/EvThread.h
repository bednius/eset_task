
#ifndef ESET_TEST_EV_THREAD_H
#define ESET_TEST_EV_THREAD_H

#include <cstdint>
#include <stack>
#include <ostream>
#include <thread>

class EvMachine;
class Code;

class EvThread {
private:
    EvMachine *_ev_machine = nullptr;
    Code *_code;
    uint64_t _ev_tid = 0;
    uint64_t _args[4];
    int64_t _registers[16];
    std::stack<uint64_t> _call_stack;
    uint64_t _instruction_ptr = 0;
public:
    EvThread(EvMachine *ev_machine, const int64_t *registers, uint32_t instruction_ptr, uint64_t ev_tid);

    explicit EvThread(EvMachine *ev_machine);

    void run();

    friend class EvMachine;

    uint64_t getTid() const;

};


#endif //ESET_TEST_EV_THREAD_H
