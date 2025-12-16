#pragma once
#include "Options.h"
#include <boost/filesystem.hpp>
#include <vector>

class DuplicateFinder {
public:
    static std::vector<std::vector<boost::filesystem::path>> find_duplicates(
        const std::vector<boost::filesystem::path>& files,
        const Options& opts
    );
};