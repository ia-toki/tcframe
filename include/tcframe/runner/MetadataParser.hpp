#pragma once

#include <istream>
#include <map>
#include <stdexcept>
#include <string>

#include "SimpleYamlParser.hpp"
#include "tcframe/os.hpp"
#include "tcframe/spec/core.hpp"
#include "tcframe/util.hpp"

using std::istreambuf_iterator;
using std::map;
using std::runtime_error;
using std::string;

namespace tcframe {

class MetadataParser {
    OperatingSystem* os_;

public:

    virtual ~MetadataParser() {}

    MetadataParser(OperatingSystem* os)
            : os_(os) {}

    virtual Metadata parse(const string& runnerPath) {
        MetadataBuilder metadata;

        string metadataPath = getMetadataPath(runnerPath);
        istream* configStream = os_->openForReading(metadataPath);
        if (!*configStream) {
            throw runtime_error(string(Metadata::METADATA_YML) + " not found");
        }

        string contents(istreambuf_iterator<char>(*configStream), {});
        map<string, string> yaml = SimpleYamlParser::parse(contents);

        if (yaml.count("slug")) {
            metadata.setSlug(yaml["slug"]);
        }
        if (yaml.count("timeLimit")) {
            optional<int> timeLimit = StringUtils::toNumber<int>(yaml["timeLimit"]);
            if (!timeLimit) {
                throwMalformed("timeLimit");
            }
            metadata.setTimeLimit(timeLimit.value());
        }
        if (yaml.count("memoryLimit")) {
            optional<int> memoryLimit = StringUtils::toNumber<int>(yaml["memoryLimit"]);
            if (!memoryLimit) {
                throwMalformed("memoryLimit");
            }
            metadata.setMemoryLimit(memoryLimit.value());
        }
        return metadata.build();
    }

private:
    static string getMetadataPath(const string& runnerPath) {
        size_t slashPos = runnerPath.find_last_of('/');
        if (slashPos != string::npos) {
            return runnerPath.substr(0, slashPos + 1) + Metadata::METADATA_YML;
        }
        return Metadata::METADATA_YML;
    }

    static void throwMalformed(const string key) {
        throw runtime_error(string(Metadata::METADATA_YML)
                            + " contains malformed YAML: cannot parse for '" + key + "'");
    }
};

}
