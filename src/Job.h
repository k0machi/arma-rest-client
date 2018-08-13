#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace ozk {
	class Job
	{
	public:
		explicit Job(std::vector<std::string>& request);
		virtual ~Job();
		std::string GetResult();
		std::vector<std::string>& GetParams();
		virtual void Execute();
		void AssignId(int newId);
		int GetId();
		bool IsComplete();
		void ParseSQFArrayAsMap(std::string& sqfArray);
	protected:
		void Complete(const std::string& result);
	private:
		bool m_completed;
		std::unordered_map<std::string, std::string> m_parameter_map;
		int m_id;
		std::vector<std::string> m_params; //deprecated
		std::string m_req;
		std::string m_result;

		//uri itself, e.g. http://example.com/api/getname/
		std::string m_uri;
		//any query parameters for urlencoding into URI, e.g. ?token=abcd&name=alice
		std::unordered_map<std::string, std::string> m_query_params;
		//any other arguments for the request itself, like type of form encoding, attached files etc.
		std::unordered_map<std::string, std::string> m_arguments;
	};

}
