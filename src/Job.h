#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace ozk {
	/**
	 * \brief Job object represents a unit that can be executed via Execute() method and does some work which then can be acquired by calling GetResult() method.
	 * All Results are of std::string type
	 */
	class Job
	{
	public:
		explicit Job(std::vector<std::string>& request);
		virtual ~Job();
		/**
		 * \brief Returns result of a job
		 * \return string containing job's result
		 */
		std::string GetResult();
		/**
		 * \brief Get Job Params
		 * \return Array of strings containing all parameters for a job
		 */
		std::vector<std::string>& GetParams();
		/**
		 * \brief Execute the job. Can itself be an async operation
		 */
		virtual void Execute();
		/**
		 * \brief Set the job id
		 * \param newId new id for the job
		 */
		void AssignId(int newId);
		/**
		 * \brief Get the job id
		 * \return job id
		 */
		int GetId();
		/**
		 * \brief Returns status of a job
		 * \return true for complete, false for incomplete job
		 */
		bool IsComplete();
		/**
		 * \brief Parses arrays passed by Arma 3 callExtension RVExtensionArray() function
		 * \param sqfArray array of strings to be parsed.
		 */
		void ParseSQFArrayAsMap(std::string& sqfArray);
	protected:
		/**
		 * \brief Set completion flag of a job and the result
		 * \param result Result of executing the job
		 */
		void Complete(const std::string& result);
	private:
		bool m_completed;
		/**
		 * \brief Parsed sqf array becomes this map
		 */
		std::unordered_map<std::string, std::string> m_parameter_map;
		int m_id;
		/**
		 * \brief Raw job parameters
		 */
		std::vector<std::string> m_params;
		/**
		 * \brief Initial Request
		 */
		std::string m_req;
		/**
		 * \brief Job result
		 */
		std::string m_result;
	};

}
