#include "textParser.h"

#include "reader.h"

#include <stdexcept>

namespace data_processing::io {

TextParser::Document TextParser::parse() const {
    return Reader::readLines(filePath_);
}

} // namespace data_processing::io
