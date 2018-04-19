#pragma once
#include <string>

namespace ozk {
	class Job
	{
	public:
		explicit Job(const std::string& request);
		~Job();
		std::string GetResult();
		std::string GetRequest();
		int GetId();
		void Complete(const std::string& result);
		bool IsComplete();
	private:
		bool m_completed;
		int m_id;
		std::string m_req;
		std::string m_result;
	};

}