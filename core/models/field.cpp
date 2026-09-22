#include "field.h"

namespace data_processing::core::models {

Field::Field(const char* value) : value_(std::string(value)) {}

Field::Field(std::string value) : value_(std::move(value)) {}

Field::Field(int value) : value_(value) {}

Field::Field(double value) : value_(value) {}

Field::Field(bool value) : value_(value) {}

bool Field::isNull() const {
    return std::holds_alternative<std::monostate>(value_);
}

bool Field::isString() const {
    return std::holds_alternative<std::string>(value_);
}

bool Field::isInt() const {
    return std::holds_alternative<int>(value_);
}

bool Field::isDouble() const {
    return std::holds_alternative<double>(value_);
}

bool Field::isBool() const {
    return std::holds_alternative<bool>(value_);
}

const Field::ValueType& Field::getValue() const {
    return value_;
}

std::string Field::toString() const {
    return std::visit([](const auto& val) -> std::string {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
            return "null";
        } else if constexpr (std::is_same_v<T, std::string>) {
            return val;
        } else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, double> || std::is_same_v<T, bool>) {
            return std::to_string(val);
        } else {
            return "unknown";
        }
    }, value_);
}

} // namespace data_processing::core::models