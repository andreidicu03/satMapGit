#ifndef TLEPARSER_H
#define TLEPARSER_H
#include "includes.hpp"

namespace fs = std::filesystem;

QList<TLEdata> SatSearch(fs::path& satPath);

#endif // TLEPARSER_H
