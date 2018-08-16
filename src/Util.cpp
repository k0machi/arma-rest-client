#include "Util.h"

using Poco::Dynamic::Var;
using Poco::JSON::Array;
using Poco::JSON::Object;

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

std::string ozk::StringifyJSONtoSQF(const std::string& json)
{
	Poco::JSON::Parser parser;
	Var result = parser.parse(json);
	std::ostringstream oss;
	//json root element is always either array or object
	SQFier::stringify(result, oss);
	return oss.str();
}

void ozk::SQFier::stringify(const Var& any, std::ostringstream& out)
{
	auto options = Poco::JSON_WRAP_STRINGS;
	if (any.type() == typeid(Object::Ptr))
	{
		Object::Ptr& o = const_cast<Object::Ptr&>(any.extract<Object::Ptr>());
		SQFier::stringifyObj(o, out);
	}
	else if (any.type() == typeid(Array::Ptr))
	{
		Array::Ptr& a = const_cast<Array::Ptr&>(any.extract<Array::Ptr>());
		SQFier::stringifyArr(a, out);
	}
	else if (any.isEmpty())
	{
		out << "null";
	}
	else if (any.isNumeric() || any.isBoolean())
	{
		std::string value = any.convert<std::string>();
		if (any.type() == typeid(char)) Poco::toJSON(value, out, options);
		else out << value;
	}
	else if (any.isString() || any.isDateTime() || any.isDate() || any.isTime())
	{
		std::string value = any.convert<std::string>();
		Poco::toJSON(value, out, options);
	}
	else
	{
		out << any.convert<std::string>();
	}
}

void ozk::SQFier::stringifyObj(const Poco::JSON::Object::Ptr& o, std::ostringstream& out)
{
	out << "[\"o\",[";

	Poco::JSON::Object::ConstIterator it = o->begin();
	Poco::JSON::Object::ConstIterator end = o->end();
	for (; it != end;)
	{
		out << "[";
		SQFier::stringify(it->first, out);
		out << ",";
		SQFier::stringify(it->second, out);;
		out << "]";

		if (++it != o->end()) out << ',';

	}
	out << "]]";
}

void ozk::SQFier::stringifyArr(const Poco::JSON::Array::Ptr& a, std::ostringstream& out)
{
	out << "[\"a\",[";

	for (Poco::JSON::Array::ValueVec::const_iterator it = a->begin(); it != a->end();)
	{
		SQFier::stringify(*it, out);

		if (++it != a->end())
		{
			out << ",";
		}
	}
	out << "]]";
}
