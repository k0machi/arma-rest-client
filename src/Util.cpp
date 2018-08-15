#include "Util.h"

void ozk::Trim(std::string& str)
{
	//ye ye mutability is bad
	//it just werks okay
	str.erase(std::remove_if(str.begin(),
		str.end(),
		[](unsigned char x) {
		switch (x)
		{
		case '\t':
		case ' ':
		case '\"':
		case '\'':
			return true;
		default:
			return false;
		}
	}), str.end());
}

Poco::Any ozk::ParseSQFArray(std::string& sqfArray, ParseMode mode)
{
	Poco::JSON::Parser parser;
	auto parsedParamArray = parser.parse(sqfArray);
	auto args = parsedParamArray.extract<Poco::JSON::Array::Ptr>();
	if (mode == ozk::ParseMode::Map)
	{
		std::unordered_map<std::string, std::string> result;
		for (int i = 0; i < args->size(); ++i) {
			auto param = args->get(i).extract<Poco::JSON::Array::Ptr>();
			auto key = param->get(0).toString();
			auto value = param->get(1).toString();
			result.emplace(key, value);
		}
		return Poco::Any(result);
	}
	else
	{
		std::vector<std::pair<std::string, std::string>> result;
		for (int i = 0; i < args->size(); ++i) {
			auto param = args->get(i).extract<Poco::JSON::Array::Ptr>();
			auto first = param->get(0).toString();
			auto second = param->get(1).toString();
			result.emplace_back(first, second);
		}
		return Poco::Any(result);
	}
}
