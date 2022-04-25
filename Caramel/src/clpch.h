#pragma once

// Defaults
#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>
#include <algorithm>
#include <functional>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>

#include "Core/Log.h"

#define BIND_FN(x) std::bind(&x, this, std::placeholders::_1)