
#ifndef ESET_TEST_ARGUMENTSPARSER_H
#define ESET_TEST_ARGUMENTSPARSER_H

#include <iostream>

class ArgumentsParser {
public:
    ArgumentsParser(int argc, char **argv);

    void checkArgs();

    const std::string &getFilename() const;

private:
    int _argc;
    char **_argv;
    std::string _filename;
};


#endif //ESET_TEST_ARGUMENTSPARSER_H
