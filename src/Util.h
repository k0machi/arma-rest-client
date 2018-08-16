#pragma once
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <utility>
#include "Poco/Any.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Object.h"

namespace ozk {
	enum class ParseMode {Pairs, Map};
	void Trim(std::string&);
	Poco::Any ParseSQFArray(std::string& sqfArray, ParseMode mode);
	std::string StringifyJSONtoSQF(const std::string& json);

	class SQFier {
	public:
		static void stringify(const Poco::Dynamic::Var&, std::ostringstream&);
	private:
		static void stringifyObj(const Poco::JSON::Object::Ptr&, std::ostringstream&);
		static void stringifyArr(const Poco::JSON::Array::Ptr&, std::ostringstream&);
	};
}