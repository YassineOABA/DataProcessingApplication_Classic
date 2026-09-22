#ifndef DATA_PROCESSING_APPLICATION_CORE_MODELS_FIELD_H
#define DATA_PROCESSING_APPLICATION_CORE_MODELS_FIELD_H

#include <string>
#include <variant>
#include <stdexcept>

namespace data_processing::core::models {

class Field {
public:
    using ValueType = std::variant<std::monostate, std::string, int, double, bool>;

    Field() = default;
    explicit Field(const char* value);
    explicit Field(std::string value);
    explicit Field(int value);
    explicit Field(double value);
    explicit Field(bool value);

    bool isNull() const;
    bool isString() const;
    bool isInt() const;
    bool isDouble() const;
    bool isBool() const;

    const ValueType& getValue() const;

    template<typename T>
    void setValue(T value) {
        value_ = std::move(value);
    }

    template<typename T>
    bool tryGetValueAs(T& value) const {
        if (auto extracted = std::get_if<T>(&value_)) {
            value = *extracted;
            return true;
        }
        return false;
    }

    template<typename T>
    T getValueAs() const {
        if (auto extracted = std::get_if<T>(&value_)) {
            return *extracted;
        }
        throw std::bad_variant_access();
    }

    std::string toString() const;

private:
    ValueType value_;
};

} // namespace data_processing::core::models

#endif // DATA_PROCESSING_APPLICATION_CORE_MODELS_FIELD_H