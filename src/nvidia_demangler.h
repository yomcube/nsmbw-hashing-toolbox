#ifndef NVIDIA_DEMANGLER_H
#define NVIDIA_DEMANGLER_H

#include <string>
#include <map>

namespace nvidia_demangler {

    void parseQClass(const std::string &, std::string &, int &);
    void parseSimpleClass(const std::string &, std::string &, int &);
    void parseArgType(const std::string &, std::string &, int &);
    void parseFunction(const std::string &, std::string &, int &);
    void parseClassOrBasicType(const std::string &, std::string &, int &);

    std::string nvidia_demangle(const std::string);
};

#endif // NVIDIA_DEMANGLER_H
