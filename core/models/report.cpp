#include "report.h"

namespace data_processing::core::models {
    
Report::Report(std::string title, std::string content) : title_(std::move(title)), content_(std::move(content)) {}

const std::string& Report::getTitle() const {
    return title_;
}

void Report::setTitle(const std::string title) {
    title_ = std::move(title);
}

const std::string& Report::getContent() const {
    return content_;
}

void Report::setContent(const std::string content) {
    content_ = std::move(content);
}

} // namespace data_processing::core::models