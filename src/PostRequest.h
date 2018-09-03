#pragma once
#include "Job.h"
#include "NetRequest.h"
#include "Util.h"

namespace ozk
{
	class POSTRequest : public Job
	{
	public:
		explicit POSTRequest(std::vector<std::string>& params);
		virtual ~POSTRequest();
		void Execute() override;
	};

}
