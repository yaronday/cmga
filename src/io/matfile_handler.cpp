#include "matfile_handler.hpp"

MatFileHandler::MatFileHandler()
    : min_rows(MIN_ROW_V), max_rows(MAX_ROW_V), row_inc(INC_V), min_cols(MIN_ROW_V),
      max_cols(MAX_ROW_V), col_inc(INC_V), min_v(MIN_V), max_v(MAX_V), sqmat(SQMAT),
      destpath(ROOTDIR), confirm(EN_CONFIRM), txt(EN_TXT), overwrite(EN_OVR), parsedMatIdx(0),
      matInfo({0, 0, 0, 0}), // n rows, m cols, min_value, max_value
      filename("") {}

const std::string MatFileHandler::updatePath(int rows, int cols, int min_value,
                                             int max_value) const {
  const std::string ext = txt ? "txt" : "csv";
  return std::format("{}/mat_{}_{}_{}_{}.{}", destpath, rows, cols, min_value, max_value, ext);
}

void MatFileHandler::updateMatParams(int min_rows, int max_rows, int row_inc, int min_cols,
                                     int max_cols, int col_inc, int min_v, int max_v) {
  this->min_rows = min_rows;
  this->max_rows = max_rows;
  this->row_inc = row_inc;
  this->min_cols = min_cols;
  this->max_cols = max_cols;
  this->col_inc = col_inc;
  this->min_v = min_v;
  this->max_v = max_v;
}

const std::string MatFileHandler::updatePath(const std::string &filename) const {

  const std::string ext = txt ? "txt" : "csv";
  size_t matRowLen = currMat.cols();
  std::string base_name = std::format("/mat_{}_{}", currMat.rows(), matRowLen);
  const std::string fname = !filename.empty() ? filename : base_name;
  return std::format("{}{}.{}", destpath, base_name, ext);
}

// parsed matrix filename updater
std::string MatFileHandler::updateStrByIntVect(std::vector<int> vect) const {
  const std::string ext = this->txt ? "txt" : "csv";
  std::string result = std::format("mat{}_", this->parsedMatIdx);
  bool first = true;
  for (const auto &val : vect) {
    if (!first)
      result += '_';
    result += std::format("{}", val);
    first = false;
  }
  result += std::format(".{}", ext);
  return result;
}

void MatFileHandler::displayMat(const Array2D<uint8_t> &mat) {
  std::string content;
  for (size_t r = 0; r < mat.rows(); ++r) {
    for (size_t c = 0; c < mat.cols(); ++c) {
      int value = static_cast<int>(mat[r][c]);
      content += std::format("{} ", value);
    }
    content += '\n';
  }
  console::formatTxt(content, LIGHT_CYAN);
}

void MatFileHandler::displayMatrices(const std::vector<Array2D<uint8_t>> &matVect) {
  for (const auto &mat : matVect) {
    displayMat(mat);
    std::string separator(2 * mat.cols(), '-');
    console::formatTxt(separator, LIGHT_CYAN);
  }
}

bool MatFileHandler::isValidMatSpec(int rows, int cols) const {
  if (rows <= 0 || cols <= 0) {
    if (rows <= 0) {
      err::handle(ErrCode::INVALID_RANGE, rows, "Rows", 1, ROWS_MAX_V);
    }
    if (cols <= 0) {
      err::handle(ErrCode::INVALID_RANGE, cols, "Columns", 1, ROWS_MAX_V);
    }
    return false;
  }
  if (!utils::isWithinRange(min_v, MIN_V, MAX_V) || !utils::isWithinRange(max_v, MIN_V, MAX_V)) {
    return false;
  }
  if (min_v > max_v) {
    err::handle(ErrCode::INVALID_RANGE, min_v, "min_value", 1, max_v);
    return false;
  }
  return true;
}

Array2D<uint8_t> MatFileHandler::genRndMat(int rows, int cols, std::mt19937 gen) const {
  if (!isValidMatSpec(rows, cols))
    return {};

  console::formatTxt(
      format("{}{}x{}{}{}\n", GEN_RAND, rows, cols, MAT_WITH_VALS, err::withinRange(min_v, max_v)),
      LIGHT_MAGENTA);

  // Instantiate Matrix2D with rows and cols dimensions
  Array2D<uint8_t> mat(rows, cols);
  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < cols; col++) {
      mat[row][col] = static_cast<uint8_t>(utils::rndIntInRange(gen, min_v, max_v));
    }
  }
  return mat;
}

Array2D<uint8_t> MatFileHandler::genRndMatWrapper(int rows, int cols,
                                                  std::optional<unsigned int> seed) const {
  std::mt19937 gen;
  if (seed) {
    gen.seed(*seed); // Use the provided seed for deterministic output
  } else {
    std::random_device rd;
    gen.seed(rd());
  }
  return genRndMat(rows, cols, gen);
}

Array2D<uint8_t> MatFileHandler::parseMat(const std::string &matrixStr) {
  std::string rowToken;
  const size_t strLength = matrixStr.length();

  if (strLength < 3) {
    err::handle(ErrCode::EMPTY_STR, err::ErrorContext());
    return Array2D<uint8_t>(1, 1, 0);
  }

  const std::string trimmedString = matrixStr.substr(1, strLength - 2);

  if (trimmedString.empty()) {
    err::handle(ErrCode::EMPTY_STR, err::ErrorContext());
    return Array2D<uint8_t>(1, 1, 0);
  }

  std::istringstream matrixStream(trimmedString);
  size_t row_len = 0;
  bool isFirstRow = true;
  std::vector<std::vector<uint8_t>> temp_rows;

  // Tokenize based on outer curly braces
  while (getline(matrixStream, rowToken, '}')) {
    if (!rowToken.empty()) {
      utils::delimFix(rowToken, "{},");
      if (std::vector<int> parsed_row = utils::validateRow(rowToken); parsed_row[0] != -1) {
        if (isFirstRow) {
          row_len = parsed_row.size();
          isFirstRow = false;
        } else if (parsed_row.size() != row_len) {
          err::handle(ErrCode::ROW_LEN_MISMATCH, err::ErrorContext());
          return Array2D<uint8_t>(1, 1, 0);
        }

        std::vector<uint8_t> uint8_row;
        uint8_row.reserve(parsed_row.size());
        for (int val : parsed_row) {
          uint8_row.push_back(static_cast<uint8_t>(val));
        }
        temp_rows.push_back(std::move(uint8_row));
      } else {
        err::handle(ErrCode::INVALID_MAT, err::ErrorContext());
        return Array2D<uint8_t>(1, 1, 0);
      }
    }
  }

  if (temp_rows.empty() || row_len == 0) {
    err::handle(ErrCode::EMPTY_STR, err::ErrorContext());
    return Array2D<uint8_t>(1, 1, 0);
  }

  const size_t num_rows = temp_rows.size();
  Array2D<uint8_t> mat(num_rows, row_len);
  for (size_t r = 0; r < num_rows; ++r) {
    for (size_t c = 0; c < row_len; ++c) {
      mat[r][c] = temp_rows[r][c];
    }
  }

  return mat;
}

std::vector<Array2D<uint8_t>> MatFileHandler::parseMatrices(const std::string &matricesStr) {
  std::vector<Array2D<uint8_t>> matrices;
  for (const std::string &matStr : utils::split(matricesStr)) {
    if (matStr != " ") {
      currMat = std::move(parseMat(matStr));
      if (!currMat.empty() && currMat[0][0]) {
        matrices.emplace_back(std::move(currMat));
      } else // fail right after the first invalid matrix / substring
        exit(1);
    }
  }
  return matrices;
}

/*calc diskspace required for N x M matrix files,
    where N = min_rows : row_inc : max_rows
          M = min_cols : col_inc : max_cols
          rowSize = M + M * delimiter + 2(CRLF)
 FileSize = N Rows * rowSize = N(2M+1)
 */
void MatFileHandler::calcSizeMatFiles() const {

  auto adj_rows = utils::adjustedRange(min_rows, max_rows, row_inc);
  auto adj_cols = utils::adjustedRange(min_cols, max_cols, col_inc);

  int r1 = min_rows;
  int rn = adj_rows.first;
  int n_rows = adj_rows.second;

  int c1 = min_cols;
  int cn = adj_cols.first;
  int n_cols = adj_cols.second;

  double sum_rows = (n_rows / 2.0) * (r1 + rn);
  double sum_cols = n_cols * (c1 + cn + 1);

  double totalSize = sum_rows * sum_cols;

  handleDiskSpace(totalSize, MAX_FILESIZE_MB);
}

// calc diskspace required for square matrix files NxN
void MatFileHandler::calcSizeSqMatFiles() const {
  auto adj_rows = utils::adjustedRange(min_rows, max_rows, row_inc);
  int rn = adj_rows.first;
  int n = adj_rows.second;

  int r1 = min_rows;
  int d = row_inc;
  int r1_sq = r1 * r1;
  int d_sq = d * d;
  int k = n * (n - 1);

  double sum_rows = (n / 2.0) * (r1 + rn);
  double s1 = n * r1_sq;
  double s2 = r1 * d * k;
  double s3 = d_sq * k * (2 * n - 1) / 6.0;
  double sum_squared_rows = s1 + s2 + s3;

  double totalSize = 2 * sum_squared_rows + sum_rows;

  handleDiskSpace(totalSize, MAX_FILESIZE_MB);
}

// Load matrix from file
// Supporting csv/txt comma/tab/semicolon separated ints
bool MatFileHandler::matLoader(Array2D<uint8_t> &matrix, const std::string &filepath) {
  std::ifstream file(filepath, std::ios::binary);
  if (!file.is_open()) {
    err::handle(ErrCode::FILE_OPEN_ERROR, err::ErrorContext());
    return false;
  }

  size_t file_size = fGetSize(filepath);

  // Read the entire file into a buffer
  std::vector<char> buffer(file_size);
  file.read(buffer.data(), file_size);
  file.close();

  size_t idx = 0;
  int row_cnt = 0;
  int col_cnt = 0;
  std::vector<uint8_t> row;
  bool in_number = false;
  bool isFirstRow = true;
  size_t row_len = 0;

  // Loop through the buffer and parse integers into rows
  while (idx < buffer.size()) {
    while (idx < buffer.size() && (buffer[idx] != '\n' && buffer[idx] != '\r')) {
      if (isdigit(buffer[idx])) { // Only process digits (1-9)
        // detect a multi-digit scenario (out of range)
        if (in_number) {
          err::handle(ErrCode::MULTIDIGIT, row_cnt, col_cnt - 1, utils::evalNum(idx - 1, buffer));
          return false;
        }

        uint8_t value = buffer[idx] - '0';

        if (value == 0) {
          err::handle(ErrCode::INVALID_RANGE, value, "Matrix", MIN_V, MAX_V);
          return false;
        }

        row.emplace_back(static_cast<uint8_t>(value));
        in_number = true;
        ++col_cnt;
      }
      // Valid delimiters
      else if (buffer[idx] == ' ' || buffer[idx] == ',' || buffer[idx] == ';' ||
               buffer[idx] == '\t') {
        in_number = false; // We're no longer in the middle of a number
        if (buffer[idx] == '\t')
          col_cnt += TABWIDTH - (col_cnt % TABWIDTH);
        else
          ++col_cnt;
      } else {
        err::handle(ErrCode::INVALID_CHAR, buffer[idx], row_cnt, col_cnt);
        return false;
      }
      ++idx;
    }

    if (!row.empty()) {
      if (isFirstRow) {
        row_len = row.size();
        matrix.emplace_back(std::move(row));
        isFirstRow = false;
      } else {
        if (row.size() != row_len) {
          err::handle(ErrCode::ROW_LEN_MISMATCH, row_cnt);
          return false;
        }
        matrix.emplace_back(std::move(row));
      }
    }

    // Skip CRLF chars at EOL
    while (idx < buffer.size() && (buffer[idx] == '\n' || buffer[idx] == '\r')) {
      in_number = false;
      if (buffer[idx] == '\n')
        ++row_cnt;
      ++idx;
    }
    // Get ready for next row
    col_cnt = 0;
    row.clear();
  }
  return true;
}

// load matrices, return them & their location
std::vector<std::pair<std::string, Array2D<uint8_t>>> MatFileHandler::fLoadMatrices() {
  std::vector<std::pair<std::string, Array2D<uint8_t>>> files;
  bool success = false;

  console::formatTxt(LOAD_MATRICES, LIGHT_CYAN);
  bool directoryEmpty = true;
  int filecount = 0;
  for (const auto &entry : std::filesystem::directory_iterator(destpath)) {
    const auto &path = entry.path();
    const auto ext = path.extension();
    std::string filename = path.filename().string();
    size_t output_file = filename.find("out");
    if ((ext == ".txt" || ext == ".csv") && output_file == std::string::npos) {
      directoryEmpty = false;
      std::string filepath = path.string();
      console::formatTxt(format("{}){}", ++filecount, filepath), LIGHT_CYAN);
      Array2D<uint8_t> mat;
      success = matLoader(mat, filepath);
      std::cout << std::endl;
      if (!mat.empty() && success) {
        files.emplace_back(filepath, std::move(mat));
      }
    }
  }
  if (directoryEmpty) {
    err::handle(ErrCode::EMPTY_DIR, destpath);
    exit(1);
  } else {
    console::formatTxt(LOAD_MAT_DONE, LIGHT_CYAN);
  }
  return files;
}

// fill buffer with generated random matrix
std::vector<char> MatFileHandler::fillBuffRndMat(int rows, int cols, std::mt19937 gen) {
  return fillBuffer([](int, int) { return true; }, // No condition, all cells are valid
                    [&](int, int) { return utils::rndCharInRange(gen, min_v, max_v); }, 0, rows - 1,
                    0, cols - 1);
}

std::vector<char> MatFileHandler::fillBuffMat() {

  if (currMat.empty()) {
    err::handle(ErrCode::MAT_EMPTY, err::ErrorContext());
    return {};
  }

  int rows = static_cast<int>(currMat.rows());
  int cols = static_cast<int>(currMat.cols());

  // No condition - all matrix cells are valid!
  auto cellCond = [](int, int) { return true; };
  auto valueCond = [&](int r, int c) { return utils::itoc(currMat[r][c]); };

  return fillBuffer(cellCond, valueCond, 0, rows - 1, 0, cols - 1);
}

std::vector<char> MatFileHandler::fillBuffRndMat(int rows, int cols) {
  std::random_device rd;
  std::mt19937 gen(rd());
  return fillBuffRndMat(rows, cols, gen);
}

// initialize a buffer
std::vector<char> MatFileHandler::bufferInit(int min_r, int max_r, int min_c, int max_c) {
  int numOfRows = max_r - min_r + 1;
  int numOfCols = max_c - min_c + 1;
  size_t buffSize = numOfRows * (2 * static_cast<size_t>(numOfCols) + 1);
  std::vector<char> buffer(buffSize);
  return buffer;
}

// write random-matrix file
void MatFileHandler::fWriteRndMat(int rows, int cols) {
  if (!isValidMatSpec(rows, cols))
    return;
  const std::string filepath = updatePath(rows, cols, min_v, max_v);
  std::string info = std::format("{}({}x{})", MATFILE_, rows, cols);
  auto buffer = fillBuffRndMat(rows, cols);
  fWrite(buffer, filepath, info, overwrite);
}

// write matrix -> buffer -> file
void MatFileHandler::fWriteMat(const std::string &filename, bool skipUpdate) {
  if (currMat.empty()) {
    err::handle(ErrCode::MAT_EMPTY, err::ErrorContext());
    return;
  }
  std::string filepath = filename;
  if (!skipUpdate)
    filepath = updatePath(filename);
  auto buffer = fillBuffMat();
  fWrite(buffer, filepath, DEFAULT_INFO, overwrite);
}

// Generate N x M matrix files
void MatFileHandler::genMatFiles() {
  calcSizeMatFiles();
  if (!confirm) {
    // files generation action
    if (utils::skipAction(FILEGEN_))
      return;
  }
  for (int rows = min_rows; rows <= max_rows; rows += row_inc) {
    for (int cols = min_cols; cols <= max_cols; cols += col_inc) {
      fWriteRndMat(rows, cols);
    }
  }
}

// Generate N x N square matrix files
void MatFileHandler::genSqMatFiles() {

  calcSizeSqMatFiles();
  if (!confirm) {
    // files generation action
    if (utils::skipAction(FILEGEN_))
      return;
  }
  for (int rows = min_rows; rows <= max_rows; rows += row_inc)
    fWriteRndMat(rows, rows);
}

void MatFileHandler::invokeGenMatFiles() {
  if (sqmat)
    genSqMatFiles();
  else
    genMatFiles();
}