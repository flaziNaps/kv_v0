#include "cli.hpp"
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <string>

struct CommandCase
{
    const char* name;
    const char* input;
    const char* expected;
    const char* forbidden = nullptr;
};

class CliTest : public testing::TestWithParam<CommandCase>
{
protected:
    void SetUp() override { previous_ = std::cout.rdbuf(output_.rdbuf()); }
    void TearDown() override { std::cout.rdbuf(previous_); }

    std::ostringstream output_;
    std::streambuf* previous_ = nullptr;
};

TEST_P(CliTest, HandlesCommands)
{
    const auto& test = GetParam();
    std::istringstream input(test.input);
    const int status = kv::cli::run(input);
    std::cout.rdbuf(previous_);
    const std::string output = output_.str();

    EXPECT_EQ(status, 0);
    EXPECT_NE(output.find(test.expected), std::string::npos) << output;
    if (test.forbidden) {
        EXPECT_EQ(output.find(test.forbidden), std::string::npos) << output;
    }
}

INSTANTIATE_TEST_SUITE_P(
    Commands, CliTest,
    testing::Values(
        CommandCase{"SetThenGet", "set name alice\nget name\nquit\n", "kvstore> alice\n"},
        CommandCase{"MissingKey", "get nope\nquit\n", "Key not found"},
        CommandCase{"EmptyInput", "\nquit\n", "No command provided"},
        CommandCase{"Overwrite", "set name alice\nset name bob\nget name\nquit\n",
                    "kvstore> bob\n", "alice"},
        CommandCase{"Delete", "set a 1\ndel a\nget a\nquit\n", "Key not found"},
        CommandCase{"DeleteMissing", "del nope\nquit\n", "del key nope failed"},
        CommandCase{"InvalidSetArguments", "set onlykey\nquit\n", "Invalid number of arguments"},
        CommandCase{"ShowAll", "set a 1\nshowAll\nquit\n", "a: 1"},
        CommandCase{"ShowEmptyStore", "showAll\nquit\n", "No data in the store."},
        CommandCase{"QuitStopsProcessing", "quit\nset x y\n", "kvstore> ", "set key sucess"},
        CommandCase{"ContainsExisting", "set a 1\ncontains a\nquit\n", " true"},
        CommandCase{"ContainsMissing", "contains nope\nquit\n", " false"},
        CommandCase{"UnknownCommand", "foo\nquit\n", "Unknown command: foo"},
        CommandCase{"Quit", "quit\n", "kvstore> ", "Unknown command"},
        CommandCase{"EndOfInput", "", "kvstore> ", "No command provided"},
        CommandCase{"CommandWithoutFinalNewline", "set a 1\nget a", "kvstore> 1\n"},
        CommandCase{"TooManyArguments", "get a b\nquit\n", "Invalid number of arguments"}),
    [](const testing::TestParamInfo<CommandCase>& info) { return info.param.name; });
