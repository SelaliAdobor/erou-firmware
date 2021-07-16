#pragma once
#include <etl/string.h>
static constexpr size_t longStringLength = 255;
static constexpr size_t shortStringLength = 25;

using LongString = etl::string<longStringLength>;
using ShortString = etl::string<shortStringLength>;