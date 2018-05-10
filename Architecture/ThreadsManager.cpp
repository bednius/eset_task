
#include <iostream>
#include "ThreadsManager.h"
#include "EvThread.h"

uint64_t ThreadsManager::s_next_id = 0;

ThreadsManager::ThreadsManager(EvMachine *ev_machine) : _ev_machine(ev_machine) {}

ThreadsManager::~ThreadsManager() {
    delete _ev_main_thread;
}


void ThreadsManager::spawnMainThread() {
    _ev_main_thread = new EvThread(_ev_machine);
    _c_main_thread = std::thread(&EvThread::run, _ev_main_thread);
    _c_main_thread.join();
}


uint64_t ThreadsManager::spawnThread(uint64_t code_addr, int64_t *registers) {
    _c_mtx.lock();
    while (_c_threads.count(s_next_id) != 0)
        s_next_id++;

    uint64_t evm_tid = s_next_id;

    _ev_mtx.lock();
    _ev_threads[evm_tid] = new EvThread(_ev_machine, registers, code_addr, evm_tid);
    _ev_mtx.unlock();

    _c_threads[evm_tid] = new std::thread(&EvThread::run, _ev_threads[evm_tid]);
    _c_mtx.unlock();

    //std::cout << evm_tid << std::endl;
    return evm_tid;
}


void ThreadsManager::joinThread(uint64_t evm_tid) {
    _c_mtx.lock();
    std::cout << "join\t" << evm_tid << std::endl;
    std::thread *c_thread = _c_threads[evm_tid];
    if (c_thread == nullptr) {
        std::cerr << "Thread: " << evm_tid << " doesn't exist" << std::endl;
        _c_mtx.unlock();
        exit(EXIT_FAILURE);
    }
    _c_mtx.unlock();

    c_thread->join();

    _c_mtx.lock();
    delete c_thread;
    _c_threads.erase(evm_tid);
    _c_mtx.unlock();

    _ev_mtx.lock();
    delete _ev_threads[evm_tid];
    _ev_threads.erase(evm_tid);
    _ev_mtx.unlock();
}


