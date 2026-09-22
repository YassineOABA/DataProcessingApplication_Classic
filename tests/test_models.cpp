#include <gtest/gtest.h>

#include <algorithm>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "core/models/dataSet.h"
#include "core/models/field.h"
#include "core/models/record.h"

TEST(FieldTests, DefaultFieldIsNull) {
    data_processing::core::models::Field field;

    EXPECT_TRUE(field.isNull());
    EXPECT_TRUE(field.isString() == false);
    EXPECT_EQ(field.toString(), "null");
}

TEST(FieldTests, StringFieldStoresStringValue) {
    data_processing::core::models::Field field("hello");

    EXPECT_TRUE(field.isString());
    EXPECT_EQ(field.getValueAs<std::string>(), "hello");
    EXPECT_EQ(field.toString(), "hello");
}

TEST(FieldTests, IntFieldStoresIntValue) {
    data_processing::core::models::Field field(42);

    EXPECT_TRUE(field.isInt());
    EXPECT_EQ(field.getValueAs<int>(), 42);
    EXPECT_EQ(field.toString(), "42");
}

TEST(FieldTests, DoubleFieldStoresDoubleValue) {
    data_processing::core::models::Field field(3.14);

    EXPECT_TRUE(field.isDouble());
    EXPECT_DOUBLE_EQ(field.getValueAs<double>(), 3.14);
    EXPECT_EQ(field.toString(), "3.140000");
}

TEST(FieldTests, BoolFieldStoresBoolValue) {
    data_processing::core::models::Field field(true);

    EXPECT_TRUE(field.isBool());
    EXPECT_TRUE(field.getValueAs<bool>());
    EXPECT_EQ(field.toString(), "1");
}

TEST(FieldTests, WrongTypeAccessFailsGracefully) {
    data_processing::core::models::Field field("hello");
    int extracted = 0;

    EXPECT_FALSE(field.tryGetValueAs(extracted));
    EXPECT_THROW(field.getValueAs<int>(), std::bad_variant_access);
}

TEST(FieldTests, TypeReplacementWorks) {
    data_processing::core::models::Field field("before");
    field.setValue(5);

    EXPECT_TRUE(field.isInt());
    EXPECT_EQ(field.getValueAs<int>(), 5);
}

TEST(RecordTests, EmptyRecordHasNoKeys) {
    data_processing::core::models::Record record;

    EXPECT_TRUE(record.getFields().empty());
    EXPECT_TRUE(record.getKeys().empty());
}

TEST(RecordTests, SetAndGetWorkForNamedFields) {
    data_processing::core::models::Record record;
    record.set("name", data_processing::core::models::Field("Alice"));
    record.set("age", data_processing::core::models::Field(30));

    EXPECT_TRUE(record.hasKey("name"));
    EXPECT_TRUE(record.hasKey("age"));
    EXPECT_EQ(record.getFields().size(), 2u);
    EXPECT_EQ(record.get("name").getValueAs<std::string>(), "Alice");
    EXPECT_EQ(record.get("age").getValueAs<int>(), 30);
}

TEST(RecordTests, MissingKeyThrowsOutOfRange) {
    data_processing::core::models::Record record;

    EXPECT_THROW(record.get("missing"), std::out_of_range);
}

TEST(RecordTests, HasValueChecksStringValues) {
    data_processing::core::models::Record record;
    record.set("status", data_processing::core::models::Field("active"));

    EXPECT_TRUE(record.hasValue("active"));
    EXPECT_FALSE(record.hasValue("inactive"));
}

TEST(RecordTests, GetKeysReturnsAllFieldNames) {
    data_processing::core::models::Record record;
    record.set("first", data_processing::core::models::Field("A"));
    record.set("second", data_processing::core::models::Field("B"));

    const auto keys = record.getKeys();
    EXPECT_EQ(keys.size(), 2u);
    EXPECT_TRUE(std::find(keys.begin(), keys.end(), "first") != keys.end());
    EXPECT_TRUE(std::find(keys.begin(), keys.end(), "second") != keys.end());
}

TEST(DataSetTests, EmptyDatasetStartsEmpty) {
    data_processing::core::models::DataSet set;

    EXPECT_TRUE(set.empty());
    EXPECT_EQ(set.size(), 0u);
    EXPECT_TRUE(set.records().empty());
}

TEST(DataSetTests, AddRecordIncrementsSize) {
    data_processing::core::models::DataSet set;
    set.addRecord(data_processing::core::models::Record());
    set.addRecord(data_processing::core::models::Record());

    EXPECT_FALSE(set.empty());
    EXPECT_EQ(set.size(), 2u);
    EXPECT_EQ(set.records().size(), 2u);
}

TEST(DataSetTests, MetadataCanBeSetAndRetrieved) {
    data_processing::core::models::DataSet set;
    set.setMetadata("source", "csv");

    EXPECT_EQ(set.getMetadata("source"), "csv");
    EXPECT_EQ(set.metadata().at("source"), "csv");
}

TEST(DataSetTests, ClearRemovesRecordsButKeepsMetadata) {
    data_processing::core::models::DataSet set;
    set.addRecord(data_processing::core::models::Record());
    set.setMetadata("version", "1");
    set.clear();

    EXPECT_EQ(set.size(), 0u);
    EXPECT_TRUE(set.empty());
    EXPECT_EQ(set.getMetadata("version"), "1");
}

TEST(DataSetTests, ConstructorCopiesInitialRecords) {
    std::vector<data_processing::core::models::Record> records;
    records.emplace_back();
    records.back().set("id", data_processing::core::models::Field(7));

    data_processing::core::models::DataSet set(records);

    EXPECT_EQ(set.size(), 1u);
    EXPECT_EQ(set.records().front().get("id").getValueAs<int>(), 7);
}
