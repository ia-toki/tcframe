#pragma once

#include <functional>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "OfficialTestCase.hpp"
#include "OfficialTestGroup.hpp"
#include "SampleTestCase.hpp"

using std::function;
using std::move;
using std::set;
using std::string;
using std::tie;

namespace tcframe {

struct RawTestSuite {
    friend class RawTestSuiteBuilder;

private:
    vector<SampleTestCase> sampleTests_;
    vector<OfficialTestGroup> officialTests_;
    function<void()> inputFinalizer_;

public:
    const vector<SampleTestCase> sampleTests() const {
        return sampleTests_;
    }

    const vector<OfficialTestGroup>& officialTests() const {
        return officialTests_;
    }

    const function<void()>& inputFinalizer() const {
        return inputFinalizer_;
    }

    bool operator==(const RawTestSuite& o) const {
        return tie(sampleTests_, officialTests_) == tie(o.sampleTests_, o.officialTests_);
    }
};

class RawTestSuiteBuilder {
private:
    RawTestSuite subject_;

    bool hasCurrentTestGroup_;
    int currentTestGroupId_;
    set<int> currentSubtaskIds_;
    vector<OfficialTestCase> currentOfficialTestCases_;

public:
    virtual ~RawTestSuiteBuilder() {}

    RawTestSuiteBuilder()
            : hasCurrentTestGroup_(false)
            , currentTestGroupId_(0)
            , currentSubtaskIds_({-1}) {

        subject_.inputFinalizer_ = []{};
    }

    RawTestSuiteBuilder& setInputFinalizer(const function<void()>& inputFinalizer) {
        subject_.inputFinalizer_ = inputFinalizer;
        return *this;
    }

    RawTestSuiteBuilder& addSampleTestCase(const vector<string>& lines, const set<int>& subtaskIds) {
        string content;
        for (const string& line : lines) {
            content += line + "\n";
        }
        subject_.sampleTests_.push_back(SampleTestCase(content, subtaskIds));

        return *this;
    }

    RawTestSuiteBuilder& addSampleTestCase(const vector<string>& lines) {
        return addSampleTestCase(lines, {-1});
    }

    RawTestSuiteBuilder& newTestGroup() {
        if (hasCurrentTestGroup_) {
            subject_.officialTests_.push_back(OfficialTestGroup(currentTestGroupId_, currentSubtaskIds_, currentOfficialTestCases_));
        }

        hasCurrentTestGroup_ = true;
        currentTestGroupId_++;
        currentSubtaskIds_ = {-1};
        currentOfficialTestCases_.clear();

        return *this;
    }

    RawTestSuiteBuilder& setSubtaskIds(const set<int>& subtaskIds) {
        currentSubtaskIds_ = subtaskIds;

        return *this;
    }

    RawTestSuiteBuilder& addOfficialTestCase(OfficialTestCase officialTestCase) {
        if (!hasCurrentTestGroup_) {
            hasCurrentTestGroup_ = true;
            currentTestGroupId_ = -1;
            currentSubtaskIds_ = {-1};
        }

        currentOfficialTestCases_.push_back(officialTestCase);

        return *this;
    }

    RawTestSuite build() {
        if (hasCurrentTestGroup_) {
            subject_.officialTests_.push_back(OfficialTestGroup(currentTestGroupId_, currentSubtaskIds_, currentOfficialTestCases_));
        }
        return move(subject_);
    }

    RawTestSuite buildWithoutLastTestGroup() {
        return move(subject_);
    }
};

}
