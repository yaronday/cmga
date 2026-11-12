#pragma once

#include <filesystem>                // IWYU pragma: keep
#include <fstream>                   // IWYU pragma: keep
#include "support/console.hpp"       // IWYU pragma: keep
#include "support/error_handler.hpp" // IWYU pragma: keep

// create dir if necessary
void createDir(const std::string &);

bool isValidDir(const std::string &);

size_t calcDirSize(const std::string &directoryPath,
                   const std::string &fnameFilter = FILENAME_FILTER);

size_t fGetSize(const std::string &);

// return filename without its extension
std::string fRemoveExt(const std::string &filename);

// generate file path, given filename, suffix and txt
std::string fGenPath(const std::string &filename, const std::string &suffix, bool txt = EN_TXT);

// Write to file
void fWrite(const std::string &contents, const std::string &filepath,
            const std::string &info = DEFAULT_INFO, bool overwrite = EN_OVR);

// Write to file directly from buffer
void fWrite(const std::vector<char> &buffer, const std::string &filepath,
            const std::string &info = DEFAULT_INFO, bool overwrite = EN_OVR);

void fHandleInfo(const std::string &filepath, const std::string &info = DEFAULT_INFO,
                 bool overwrite = EN_OVR);

void handleDiskSpace(double totalSize, const double maxSize);

template <typename T> inline std::string to_str_w_precision(const T val, const int prec = 2) {
  return std::format("{:.{}f}", val, prec);
}