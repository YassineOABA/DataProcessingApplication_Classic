#ifndef DATA_PROCESSING_APPLICATION_CORE_MODELS_REPORT_H
#define DATA_PROCESSING_APPLICATION_CORE_MODELS_REPORT_H

#include <string>
#include <vector>

namespace data_processing::core::models {

class Report {
public:
    Report(std::string title, std::string content = "");
    const std::string& getTitle() const;
    void setTitle(const std::string title);
    const std::string& getContent() const;
    void setContent(const std::string content);
private:
    std::string title_;
    std::string content_;
};

} // namespace data_processing::core::models

#endif // DATA_PROCESSING_APPLICATION_CORE_MODELS_REPORT_H