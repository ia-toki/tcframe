#include "gmock/gmock.h"
#include "../mock.hpp"

#include "../os/MockOperatingSystem.hpp"
#include "tcframe/runner/ConfigParser.hpp"

using std::ios_base;

using ::testing::Eq;
using ::testing::Return;
using ::testing::Test;

namespace tcframe {

class ConfigParserTests : public Test {
protected:
    Mock(OperatingSystem) os;

    ConfigParser parser = ConfigParser(&os);
};

TEST_F(ConfigParserTests, Parse_Successful) {
    string yaml =
            "timeLimit: 3\n"
            "memoryLimit: 256\n";
    ON_CALL(os, openForReading("./contest/foo/foo.yml")).WillByDefault(Return(new istringstream(yaml)));

    Config config = parser.parse("./contest/foo/foo");
    EXPECT_THAT(config.slug(), Eq("foo"));
    EXPECT_THAT(config.timeLimit(), Eq(optional<int>(3)));
    EXPECT_THAT(config.memoryLimit(), Eq(optional<int>(256)));
}

TEST_F(ConfigParserTests, Parse_Failed_NoConfigFile) {
    istream* notFound = new istringstream();
    notFound->setstate(ios_base::failbit);
    ON_CALL(os, openForReading("./contest/foo/foo.yml")).WillByDefault(Return(notFound));

    Config config = parser.parse("./contest/foo/foo");
    EXPECT_THAT(config.slug(), Eq("foo"));
    EXPECT_THAT(config.timeLimit(), Eq(optional<int>()));
    EXPECT_THAT(config.memoryLimit(), Eq(optional<int>()));
}

TEST_F(ConfigParserTests, Parse_Failed_InvalidYaml) {
    string yaml =
            "timeLimit: 3\n"
            "memoryLimit: abc\n";
    ON_CALL(os, openForReading("./contest/foo/foo.yml")).WillByDefault(Return(new istringstream(yaml)));

    Config config = parser.parse("./contest/foo/foo");
    EXPECT_THAT(config.slug(), Eq("foo"));
    EXPECT_THAT(config.timeLimit(), Eq(optional<int>(3)));
    EXPECT_THAT(config.memoryLimit(), Eq(optional<int>()));
}

}
