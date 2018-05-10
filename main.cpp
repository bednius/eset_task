#include <iostream>
#include "ArgumentsParser.h"
#include "Architecture/EvMachine.h"


int main(int argc, char **argv) {

    ArgumentsParser argumentsParser(argc, argv);
    argumentsParser.checkArgs();

    EvMachine machine(argumentsParser.getFilename());
    machine.start();

    return 0;
}


