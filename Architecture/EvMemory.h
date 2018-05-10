
#ifndef ESET_TEST_EV_MEMORY_H
#define ESET_TEST_EV_MEMORY_H

#include <ostream>

class EvMemory {
public:
    explicit EvMemory(uint32_t pMemSize);

    virtual ~EvMemory();

    void fill_mem(uint8_t *data, uint32_t bytes_num);

    uint64_t read_bytes(uint32_t address, uint32_t bytes_num);

    void read_bytes(uint32_t address, uint32_t bytes_num, uint8_t *buff);

    void write_bytes(uint32_t address, uint32_t bytes_num, uint64_t val);

    void write_bytes(uint32_t address, uint32_t bytes_num, uint8_t *buff);

private:
    uint8_t *_data = nullptr;

    uint32_t _size;
};


#endif //ESET_TEST_EV_MEMORY_H
