
#include "ArgumentsParser.h"


ArgumentsParser::ArgumentsParser(int argc, char **argv) : _argc(argc), _argv(argv) {}


const std::string &ArgumentsParser::getFilename() const {
    return _filename;
}


void ArgumentsParser::checkArgs() {
    if (_argc == 2) {
        _filename = _argv[1];
    } else {
        std::cerr << "Expected one argument - filename" << std::endl
                  << "Example input - crc.evm" << std::endl
                  << "Input/output binary files for crc.evm are - crc.in/crc.out" << std::endl;
        exit(EXIT_FAILURE);
    }
}
