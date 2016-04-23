#pragma once

#include "gmock/gmock.h"

using ::testing::NiceMock;

#define Mock(T) NiceMock<Mock ## T>
