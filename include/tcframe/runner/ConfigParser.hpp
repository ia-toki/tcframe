#pragma once

#include <istream>
#include <map>
#include <stdexcept>
#include <string>

#include "SimpleYamlParser.hpp"
#include "tcframe/spec/core.hpp"
#include "tcframe/os.hpp"
#include "tcframe/util.hpp"

using std::istreambuf_iterator;
using std::map;
using std::string;

namespace tcframe {

class ConfigParser {
private:
    OperatingSystem* os_;

public:
    virtual ~ConfigParser() {}

    ConfigParser(OperatingSystem* os)
            : os_(os) {}

    virtual Config parse(const string& runnerFilename) {
        string slug = getSlug(runnerFilename);
        ConfigBuilder config(slug);

        string configFilename = getConfigFilename(runnerFilename);
        istream* configStream = os_->openForReading(configFilename);
        if (*configStream) {
            string contents(istreambuf_iterator<char>(*configStream), {});
            map<string, string> yaml = SimpleYamlParser::parse(contents);

            if (yaml.count("timeLimit")) {
                optional<int> timeLimit = StringUtils::toNumber<int>(yaml["timeLimit"]);
                if (timeLimit) {
                    config.setTimeLimit(timeLimit.value());
                }
            }
            if (yaml.count("memoryLimit")) {
                optional<int> memoryLimit = StringUtils::toNumber<int>(yaml["memoryLimit"]);
                if (memoryLimit) {
                    config.setMemoryLimit(memoryLimit.value());
                }
            }
        }
        return config.build();
    }

private:
    static string getConfigFilename(const string& runnerFilename) {
        return runnerFilename + ".yml";
    }

    static string getSlug(const string& runnerFilename) {
        size_t slashPos = runnerFilename.find_last_of('/');
        if (slashPos != string::npos) {
            return runnerFilename.substr(slashPos + 1, runnerFilename.length() - slashPos);
        }
        return runnerFilename;
    }
};

}
