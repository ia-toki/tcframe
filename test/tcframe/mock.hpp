#pragma once

#include "gmock/gmock.h"

using ::testing::NiceMock;

#define MOCK(T) NiceMock<Mock ## T>
