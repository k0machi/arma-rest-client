#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

namespace ozk {
	enum class JobStatus
	{
		CALL_SUCCESS = 0x0,
		CALL_E_GENERIC = 0x0f,
		JOB_COMPLETE = 0x10,
		JOB_INCOMPLETE = 0x11,
		RESULT_COMPLETE = 0x20,
		RESULT_COMPLETE_JSON = 0x21,
		RESULT_SLICED = 0x22,
		RESULT_NOTREADY = 0x2f,
		RESULT_INVALIDINPUT = 0x30,
		RESULT_EXCEPTION_CAUGHT = 0x31,
		DEBUG_INFO_DUMP = 0xff
	};

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
		 * \brief Return offset number of result string. Used for handling large results.
		 * \return 
		 */
		int GetResultOffset();
		/**
		 * \brief Update result's offset to a new value. Returns new value
		 * \param newOffset new offset
		 */
		void UpdateResultOffset(int newOffset);
		void ResetResultOffset();
		/*
		* \brief If there were exceptions caught during processing of this job, this will return false
		* \return bool
		*/
		bool IsValid();
		/*
		* \brief Status of this work that will be returned to arma
		*/
		JobStatus GetStatus();
	protected:
		/**
		 * \brief Set completion flag of a job and the result
		 * \param result Result of executing the job
		 */
		void Complete(const std::string& result, JobStatus status = JobStatus::RESULT_COMPLETE);
		std::string GetURL();
		//uri itself, e.g. http://example.com/api/getname/
		std::string m_query_target;
		//any query parameters for urlencoding into URI, e.g. ?token=abcd&name=alice
		std::vector<std::pair<std::string, std::string>> m_query_params {};
		//any other arguments for the request itself, like type of form encoding, attached files etc.
		std::unordered_map<std::string, std::string> m_query_arguments {};
		/**
		* \brief Job result
		*/
		std::string m_result;
		bool m_completed;
		bool m_valid{ true };
		JobStatus m_status;
	private:
		int m_result_offset;
		std::unordered_map<std::string, std::string> m_parameter_map;
		int m_id;
		/**
		 * \brief Initial Request
		 */
		std::string m_req;
	};

}
