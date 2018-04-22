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
		std::vector<std::string> m_params;
		std::string m_req;
		std::string m_result;
	};

}
