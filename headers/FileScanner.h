// headers/FileScanner.h
#pragma once
#include "Options.h"
#include <vector>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

class FileScanner {
public:
   static std::vector<fs::path> scan(const Options& opts);
};