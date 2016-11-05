#include "gmock/gmock.h"
#include "../mock.hpp"

#include "../os/MockOperatingSystem.hpp"
#include "tcframe/runner/MetadataParser.hpp"

using std::ios_base;

using ::testing::Eq;
using ::testing::Return;
using ::testing::Test;

namespace tcframe {

class MetadataParserTests : public Test {
protected:
    MOCK(OperatingSystem) os;

    MetadataParser parser = MetadataParser(&os);
};

TEST_F(MetadataParserTests, Parse_Successful) {
    string yaml =
            "timeLimit: 3\n"
            "memoryLimit: 256\n";
    ON_CALL(os, openForReading("./contest/foo/foo.yml")).WillByDefault(Return(new istringstream(yaml)));

    Metadata metadata = parser.parse("./contest/foo/foo");
    EXPECT_THAT(metadata.slug(), Eq("foo"));
    EXPECT_THAT(metadata.timeLimit(), Eq(optional<int>(3)));
    EXPECT_THAT(metadata.memoryLimit(), Eq(optional<int>(256)));
}

TEST_F(MetadataParserTests, Parse_Failed_NoConfigFile) {
    istream* notFound = new istringstream();
    notFound->setstate(ios_base::failbit);
    ON_CALL(os, openForReading("./contest/foo/foo.yml")).WillByDefault(Return(notFound));

    Metadata metadata = parser.parse("./contest/foo/foo");
    EXPECT_THAT(metadata.slug(), Eq("foo"));
    EXPECT_THAT(metadata.timeLimit(), Eq(optional<int>()));
    EXPECT_THAT(metadata.memoryLimit(), Eq(optional<int>()));
}

TEST_F(MetadataParserTests, Parse_Failed_InvalidYaml) {
    string yaml =
            "timeLimit: 3\n"
            "memoryLimit: abc\n";
    ON_CALL(os, openForReading("./contest/foo/foo.yml")).WillByDefault(Return(new istringstream(yaml)));

    Metadata metadata = parser.parse("./contest/foo/foo");
    EXPECT_THAT(metadata.slug(), Eq("foo"));
    EXPECT_THAT(metadata.timeLimit(), Eq(optional<int>(3)));
    EXPECT_THAT(metadata.memoryLimit(), Eq(optional<int>()));
}

}
