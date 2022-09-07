#include "nvidia_demangler.h"

// Replication of the CW ABI demangler used by NVIDIA for the NVIDIA Shield ports
// written by RootCubed

// Python version at https://gist.github.com/RootCubed/9ebecf21eec344f10164cdfabbf0bb41

#include <iostream>
#include <cctype>

// Demangler bug: Doesn't support types like x (long long)
std::map<char, std::string> basicTypes = {
    {'v', "void"},
    {'b', "bool"},
    {'c', "char"},
    {'s', "short"},
    {'i', "int"},
    {'l', "long"},
    {'f', "float"},
    {'d', "double"},
    {'w', "wchar_t"}
};

void nvidia_demangler::parseClassOrBasicType(const std::string &mangled, std::string &out, int &pos) {
    if (mangled[pos] == 'Q') {
        parseQClass(mangled, out, ++pos);
    } else if (std::isdigit(mangled[pos])) {
        parseSimpleClass(mangled, out, pos);
    } else {
        if (mangled[pos] == 'U') {
            out += "unsigned " + basicTypes.at(mangled[++pos]);
        } else {
            out += basicTypes.at(mangled[pos]);
        }
        pos++;
    }
}
void nvidia_demangler::parseQClass(const std::string &mangled, std::string &out, int &pos) {
    int count = mangled.at(pos) - '0';
    pos++;
    for (int i = 0; i < count; i++) {
        parseSimpleClass(mangled, out, pos);
        if (i < count - 1) out += "::";
    }
}
void nvidia_demangler::parseSimpleClass(const std::string &mangled, std::string &out, int &pos) {
    int size = 0;
    while (pos < mangled.size() && std::isdigit(mangled[pos])) {
        size *= 10;
        size += (mangled[pos] - '0');
        pos++;
    }

    int end = pos + size;
    while (pos < end) {
        char c = mangled.at(pos);
        out += c;
        pos++;
        if (c == '<') {
            // Demangler bug: The demangler assumes one template parameter only
            // Demangler bug: No checks for literal values as template arguments
            parseArgType(mangled, out, pos);
        }
    }
}

void nvidia_demangler::parseArgType(const std::string &mangled, std::string &out, int &pos) {
    // Demangler bug: type modifiers are handled incorrectly
    bool isConst = false;
    bool isPtr = false;
    bool isRef = false;
    while (pos < mangled.size()) {
        char c = mangled[pos];

        // Demangler bug: M (PTMFS) and A (arrays) are not handled
        if (c == 'C') {
            isConst = true;
        } else if (c == 'P') {
            isPtr = true;
        } else if (c == 'R') {
            isRef = true;
        } else if (c == 'F') {
            // Demangler bug: Demangler was built without function pointers in mind, so they are incorrectly handled
            out += "( ";
            try {
                parseFunction(mangled, out, ++pos);
            } catch (const std::exception &e) {
                out += " )";
                throw e;
            }
            out += " )";
            break;
        } else {
            break;
        }
        pos++;
    }
    if (isConst) out += "const ";
    std::string typeName = "";
    try {
        parseClassOrBasicType(mangled, typeName, pos);
    } catch (const std::out_of_range &e) {
        if (typeName != "") {
            out += typeName;
            if (isPtr) out += "*";
            if (isRef) out += "&";
        }
        throw e;
    }
    out += typeName;
    // Demangler bug: The order of R and P does not matter
    if (isPtr) out += "*";
    if (isRef) out += "&";
}

void nvidia_demangler::parseFunction(const std::string &mangled, std::string &out, int &pos) {
    while (pos < mangled.size()) {
        parseArgType(mangled, out, pos);
        if (pos < mangled.size()) {
            out += ", ";
        }
    }
}

std::string nvidia_demangler::nvidia_demangle(const std::string mangled) {
    std::string funcName;
    int i = 0;
    while (i < mangled.size()) {
        funcName += mangled[i];
        i++;
        // Followed by /__[CFQ0-9]/?
        if (i <= mangled.size() - 3 && mangled[i] == '_' && mangled[i + 1] == '_') {
            char cAfter = mangled[i + 2];
            if (cAfter == 'C' || cAfter == 'F' || cAfter == 'Q' || std::isdigit(cAfter)) break;
        }
    }
    if (i == mangled.size()) return funcName;

    // Skip past __
    i += 2;

    std::string res;

    try {
        // Check if class method or global function
        if (mangled[i] != 'F' && mangled[i] != 'C') {
            parseClassOrBasicType(mangled, res, i);
            res += "::";
        }
        res += funcName;
    } catch(const std::exception &) {}

    if (i == mangled.size()) return res;

    // Probably not how NVIDIA did it, but I can't get it to work by supporting const functions in parseArgType
    bool isConst = false;
    if (mangled[i] == 'C') {
        isConst = true;
        i++;
    }

    try {
        parseArgType(mangled, res, i);
    } catch(const std::exception &) {}
    if (isConst) res += " const";

    return res;
}
