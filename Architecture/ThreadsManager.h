
#ifndef ESET_TEST_THREADS_MANAGER_H
#define ESET_TEST_THREADS_MANAGER_H

#include <cstdint>
#include <map>
#include <mutex>
#include <thread>

class EvThread;

class EvMachine;

class ThreadsManager {
public:
    explicit ThreadsManager(EvMachine *ev_machine);

    virtual ~ThreadsManager();

    void spawnMainThread();

    uint64_t spawnThread(uint64_t code_addr, int64_t *registers);

    void joinThread(uint64_t evm_tid);

private:
    static uint64_t s_next_id;

    EvMachine *_ev_machine;

    EvThread *_ev_main_thread = nullptr;

    std::map<uint64_t, EvThread *> _ev_threads;

    std::map<uint64_t, std::thread *> _c_threads;

    std::thread _c_main_thread;

    std::mutex _ev_mtx;

    std::mutex _c_mtx;
};


#endif //ESET_TEST_THREADS_MANAGER_H
