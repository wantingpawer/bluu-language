#include "common.h"

cfgReader::cfgReader(std::string cfgFile){
    this->cfgFile = cfgFile;
    this->parseCfg();
}
