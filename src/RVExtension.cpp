#include <string>
#include <vector>
#include <iterator>
#include "Scheduler.h"
#include "GetRequest.h"
#include "Worker.h"

std::vector<ozk::Worker*> g_ExtensionWorkers;

const int gc_numWorkers = 3;
const char gc_version[] = "1.0.0.0";
const char gc_deprecatedInterfaceMessage[] = "Usage of RVExtension interface is not supported, please use callExtension array syntax";

void InitializeWorkers() {
	if (g_ExtensionWorkers.size() != gc_numWorkers) {
		for (size_t i = 0; i < gc_numWorkers; i++)
		{
			g_ExtensionWorkers.push_back(new ozk::Worker());
		}
	}
}

extern "C"
{
	//--- Engine called on extension load 
	__declspec (dllexport) void __stdcall RVExtensionVersion(char *output, int outputSize);
	//--- STRING callExtension STRING
	__declspec (dllexport) void __stdcall RVExtension(char *output, int outputSize, const char *function);
	//--- STRING callExtension ARRAY
	__declspec (dllexport) int __stdcall RVExtensionArgs(char *output, int outputSize, const char *function, const char **args, int argsCnt);
}

void RVExtensionVersion(char * output, int outputSize)
{
	InitializeWorkers();

	strcpy_s(output, outputSize, gc_version);
}

void RVExtension(char * output, int outputSize, const char * function)
{
	strcpy_s(output, outputSize, gc_deprecatedInterfaceMessage);
}

int RVExtensionArgs(char * output, int outputSize, const char * function, const char ** args, int argsCnt)
{
	const std::string func = function;
	std::vector<std::string> vecArgs(args, std::next(args, argsCnt));
	if (func == "GETRequest") {
		auto job = new ozk::GETRequest(vecArgs);
		auto id = ozk::Scheduler::GetInstance()->AddJob(job);
		return id;
	} 
	if (func == "CheckJob") {
		auto id = std::stoi(vecArgs[0]);
		if (ozk::Scheduler::GetInstance()->GetCompletedJob(id)) {
			return 0;
		} else {
			return 1;
		}
	}
	if (func == "GetResult") {
		auto id = std::stoi(vecArgs[0]);
		ozk::Job* completedJob;
		if ((completedJob = ozk::Scheduler::GetInstance()->GetCompletedJob(id))) {
			strcpy_s(output, outputSize, completedJob->GetResult().c_str());
			return 0;
		} else {
			return 1;
		}
	}
	return 0xff;
}
