#ifndef TLEPARSER_H
#define TLEPARSER_H
#include "includes.h"

using namespace std;
namespace fs = std::filesystem;

struct SearchBuffer {
    string type, filename;
};

vector<TLEdata> SatSearch(fs::path& satPath);

#endif // TLEPARSER_H
