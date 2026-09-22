#include <gtest/gtest.h>

#include <string>
#include <unordered_map>

#include "utils/config.h"
#include "utils/logger.h"

TEST(ConfigTests, DefaultConfigIsEmpty) {
    data_processing::utils::Config config;

    EXPECT_TRUE(config.getAll().empty());
    EXPECT_EQ(config.getValue("missing"), "");
}

TEST(ConfigTests, SetAndGetWorkForExistingValues) {
    data_processing::utils::Config config;
    config.setValue("key1", "value1");
    config.setValue("key2", "value2");

    EXPECT_EQ(config.getValue("key1"), "value1");
    EXPECT_EQ(config.getValue("key2"), "value2");
    EXPECT_EQ(config.getValue("missing"), "");
}

TEST(ConfigTests, ConstructorInitializesFromMap) {
    std::unordered_map<std::string, std::string> initialMap = {{"key1", "value1"}, {"key2", "value2"}};
    data_processing::utils::Config config(initialMap);

    EXPECT_EQ(config.getValue("key1"), "value1");
    EXPECT_EQ(config.getValue("key2"), "value2");
}

TEST(ConfigTests, OverwriteExistingKeyReplacesValue) {
    data_processing::utils::Config config;
    config.setValue("key1", "value1");
    config.setValue("key1", "new_value");

    EXPECT_EQ(config.getValue("key1"), "new_value");
}

TEST(ConfigTests, ReturnedMapIsACopy) {
    data_processing::utils::Config config;
    config.setValue("key1", "value1");

    auto copied = config.getAll();
    copied["key1"] = "modified";

    EXPECT_EQ(config.getValue("key1"), "value1");
}

TEST(ConfigTests, EmptyKeyAndEmptyValueAreHandled) {
    data_processing::utils::Config config;
    config.setValue("", "");
    config.setValue("key", "value");

    EXPECT_EQ(config.getValue(""), "");
    EXPECT_EQ(config.getValue("key"), "value");
}

TEST(ConfigTests, SpecialCharactersAndLongValuesAreHandled) {
    std::string longKey(1000, 'k');
    std::string longValue(1000, 'v');
    data_processing::utils::Config config;
    config.setValue("key!@#$%^&*()", "value!@#$%^&*()");
    config.setValue(longKey, longValue);

    EXPECT_EQ(config.getValue("key!@#$%^&*()"), "value!@#$%^&*()");
    EXPECT_EQ(config.getValue(longKey), longValue);
}

TEST(LoggerTests, LogInfoMessageWritesToStdout) {
    data_processing::utils::Logger logger;
    testing::internal::CaptureStdout();
    logger.logInfo("This is an info message.");

    const std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("INFO: This is an info message."), std::string::npos);
}

TEST(LoggerTests, LogErrorMessageWritesToStderr) {
    data_processing::utils::Logger logger;
    testing::internal::CaptureStderr();
    logger.logError("This is an error message.");

    const std::string output = testing::internal::GetCapturedStderr();
    EXPECT_NE(output.find("ERROR: This is an error message."), std::string::npos);
}

TEST(LoggerTests, LogDebugAndWarningMessagesAreFormatted) {
    data_processing::utils::Logger logger;
    testing::internal::CaptureStdout();
    logger.logDebug("Debug message.");
    logger.logWarning("Warning message.");

    const std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("DEBUG: Debug message."), std::string::npos);
    EXPECT_NE(output.find("WARNING: Warning message."), std::string::npos);
}

TEST(LoggerTests, MixedMessagesGoToCorrectStreams) {
    data_processing::utils::Logger logger;
    testing::internal::CaptureStdout();
    testing::internal::CaptureStderr();
    logger.logInfo("Info message.");
    logger.logError("Error message.");

    const std::string stdoutOutput = testing::internal::GetCapturedStdout();
    const std::string stderrOutput = testing::internal::GetCapturedStderr();
    EXPECT_NE(stdoutOutput.find("INFO: Info message."), std::string::npos);
    EXPECT_NE(stderrOutput.find("ERROR: Error message."), std::string::npos);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
