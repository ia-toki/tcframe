#include "gmock/gmock.h"
#include "../mock.hpp"

#include "../os/MockOperatingSystem.hpp"
#include "tcframe/runner/MetadataParser.hpp"

using std::ios_base;

using ::testing::Eq;
using ::testing::Return;
using ::testing::StrEq;
using ::testing::Test;

namespace tcframe {

class MetadataParserTests : public Test {
protected:
    MOCK(OperatingSystem) os;

    MetadataParser parser = MetadataParser(&os);
};

TEST_F(MetadataParserTests, Parse_Successful) {
    string yaml =
            "slug: foo\n"
            "timeLimit: 3\n"
            "memoryLimit: 256\n";
    ON_CALL(os, openForReading("./contest/a-foo/metadata.yml")).WillByDefault(Return(new istringstream(yaml)));

    Metadata metadata = parser.parse("./contest/a-foo/runner");
    EXPECT_THAT(metadata.slug(), Eq("foo"));
    EXPECT_THAT(metadata.timeLimit(), Eq(optional<int>(3)));
    EXPECT_THAT(metadata.memoryLimit(), Eq(optional<int>(256)));
}

TEST_F(MetadataParserTests, Parse_Failed_NoMetadataFile) {
    istream* notFound = new istringstream();
    notFound->setstate(ios_base::failbit);
    ON_CALL(os, openForReading("./contest/a-foo/metadata.yml")).WillByDefault(Return(notFound));

    try {
        parser.parse("./contest/a-foo/runner");
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("metadata.yml not found"));
    }
}

TEST_F(MetadataParserTests, Parse_Failed_NoSlug) {
    string yaml =
            "timeLimit: 3\n"
            "memoryLimit: 256\n";
    ON_CALL(os, openForReading("./contest/a-foo/metadata.yml")).WillByDefault(Return(new istringstream(yaml)));

    try {
        parser.parse("./contest/a-foo/runner");
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("metadata.yml must contain 'slug'"));
    }
}

TEST_F(MetadataParserTests, Parse_Failed_MalformedYaml) {
    string yaml =
            "slug: foo\n"
            "timeLimit: 3\n"
            "memoryLimit: abc\n";
    ON_CALL(os, openForReading("./contest/a-foo/metadata.yml")).WillByDefault(Return(new istringstream(yaml)));

    try {
        parser.parse("./contest/a-foo/runner");
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("metadata.yml contains malformed YAML: cannot parse for 'memoryLimit'"));
    }
}

}
