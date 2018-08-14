#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

namespace ozk {
	class Job
	{
	public:
		explicit Job(std::vector<std::string>& request);
		virtual ~Job();
		std::string GetResult();
		virtual void Execute();
		void AssignId(int newId);
		int GetId();
		bool IsComplete();
	protected:
		void Complete(const std::string& result);
		std::string GetURL();
		//uri itself, e.g. http://example.com/api/getname/
		std::string m_query_target;
		//any query parameters for urlencoding into URI, e.g. ?token=abcd&name=alice
		std::vector<std::pair<std::string, std::string>> m_query_params;
		//any other arguments for the request itself, like type of form encoding, attached files etc.
		std::unordered_map<std::string, std::string> m_query_arguments;
	private:
		bool m_completed;
		std::unordered_map<std::string, std::string> m_parameter_map;
		int m_id;
		std::string m_req;
		std::string m_result;
	};

}
