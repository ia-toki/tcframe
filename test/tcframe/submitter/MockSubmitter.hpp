#pragma once

#include "gmock/gmock.h"

#include "tcframe/submitter/Submitter.hpp"

namespace tcframe {

class MockSubmitter : public Submitter {
public:
    MockSubmitter()
            : Submitter(nullptr, nullptr) {}

    MOCK_METHOD3(submit, void(const TestSuite&, const set<int>&, const SubmitterConfig&));
};

class MockSubmitterFactory : public SubmitterFactory {
public:
    MOCK_METHOD2(create, Submitter*(TestCaseSubmitter*, SubmitterLogger*));
};

}
