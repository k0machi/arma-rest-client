#pragma once
#include "Job.h"
#include "NetRequest.h"

namespace ozk
{
	class GETRequest : public Job, public NetRequest
	{
	public:
		explicit GETRequest(std::vector<std::string>& params);
		virtual ~GETRequest();
		/**
		 * \brief Execute the GET request, await response and store the result
		 */
		void Execute() override;
	};

}
