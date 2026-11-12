#include "file_utils.hpp"

// calc directory size except for file names specified by filenameFilter
size_t calcDirSize(const std::string &directoryPath, const std::string &fnameFilter) {
  size_t dirByteSize = 0;
  try {
    for (const auto &entry : std::filesystem::recursive_directory_iterator(directoryPath)) {
      const auto &path = entry.path();
      const auto ext = path.extension();
      std::string filename = path.filename().string();
      size_t output_file = filename.find(fnameFilter);
      if (std::filesystem::is_regular_file(entry) && output_file == std::string::npos) {
        dirByteSize += std::filesystem::file_size(entry);
      }
    }
  } catch (const std::exception &e) {
    err::handle(ErrCode::GENERIC_EXCEPTION, e);
    exit(1);
  }
  return dirByteSize;
}

// create dir if necessary
void createDir(const std::string &root_dir) {
  try {
    if (!std::filesystem::exists(root_dir)) {
      if (!std::filesystem::create_directories(root_dir)) {
        err::handle(ErrCode::CDIR_FAILURE, root_dir);
        exit(1);
      }
    }
    return;
  } catch (const std::exception &e) {
    err::handle(ErrCode::GENERIC_EXCEPTION, e);
    exit(1);
  }
}

bool isValidDir(const std::string &dir) {
  if (!std::filesystem::is_directory(dir)) {
    err::handle(ErrCode::INVALID_DIR, dir);
    return false;
  }
  return true;
}

size_t fGetSize(const std::string &filepath) {
  std::ifstream file(filepath, std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    err::handle(ErrCode::FILE_OPEN_ERROR, err::ErrorContext());
    exit(1);
  }
  return static_cast<size_t>(file.tellg());
}

// Write to file
void fWrite(const std::string &contents, const std::string &filepath, const std::string &info,
            bool overwrite) {
  if (contents.empty())
    return;
  fHandleInfo(filepath, info, overwrite);
  std::ofstream file(filepath);
  if (!file.is_open()) {
    err::handle(ErrCode::FILE_OPEN_ERROR, err::ErrorContext());
    return;
  }
  file << contents;
  file.close();
}

// Write to file directly from buffer
void fWrite(const std::vector<char> &buffer, const std::string &filepath, const std::string &info,
            bool overwrite) {
  if (buffer.empty())
    return;
  fHandleInfo(filepath, info, overwrite);
  std::ofstream file(filepath, std::ios::binary);
  if (!file.is_open()) {
    err::handle(ErrCode::FILE_OPEN_ERROR, err::ErrorContext());
    return;
  }
  file.write(buffer.data(), buffer.size());
  file.close();
}

void fHandleInfo(const std::string &filepath, const std::string &info, bool overwrite) {
  std::ifstream infile(filepath);
  bool fileExists = infile.good();
  infile.close();
  if ((!overwrite) && fileExists) {
    err::handle(ErrCode::OVR_NOT_ENABLED, err::ErrorContext(filepath));
    return;
  }

  const std::string actionPrefix = fileExists ? OVR_ : WR_;
  std::string msg = std::format("{} {} {}{}", actionPrefix, info, COLON_SP, filepath);
  console::formatTxt(msg, LIGHT_GREEN);
}

std::string fGenPath(const std::string &filename, const std::string &suffix, bool txt) {
  const std::string ext = txt ? "txt" : "csv";
  return std::format("{}{}.{}", fRemoveExt(filename), suffix, ext);
}

std::string fRemoveExt(const std::string &filename) {
  return filename.substr(0, filename.length() - EXT_LEN - 1);
}

void handleDiskSpace(double totalSize, const double maxSize) {
  double totalSizeKB = totalSize / KB_SIZE;
  double totalSizeMB = totalSizeKB / KB_SIZE;
  double totalSizeGB = totalSizeMB / KB_SIZE;

  // Disk space protection
  if (totalSizeMB > maxSize) {
    err::handle(ErrCode::FILESIZE_LIMIT_REACHED, totalSizeMB, maxSize);
    exit(0);
  }

  console::formatTxt(REQ_SPACE, LIGHT_YELLOW, "");
  const std::string space_required =
      (totalSizeGB >= 1)   ? to_str_w_precision(totalSizeGB) + GB_UNIT
      : (totalSizeMB >= 1) ? to_str_w_precision(totalSizeMB) + MB_UNIT
      : (totalSizeKB >= 1) ? to_str_w_precision(totalSizeKB) + KB_UNIT
                           : std::to_string(static_cast<int>(totalSize)) + BYTES_UNIT;

  console::formatTxt(space_required, LIGHT_YELLOW);
}