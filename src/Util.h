#pragma once
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <utility>
#include "Poco/Any.h"
#include <Poco/JSON/Parser.h>

namespace ozk {
	enum class ParseMode {Pairs, Map};
	void Trim(std::string&);
	Poco::Any ParseSQFArray(std::string& sqfArray, ParseMode mode);
}