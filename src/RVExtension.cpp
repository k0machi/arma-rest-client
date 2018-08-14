#include <string>
#include <vector>
#include <iterator>
#include "Scheduler.h"
#include "GetRequest.h"
#include "Worker.h"

std::vector<ozk::Worker*> g_ExtensionWorkers;

/**
 * \brief Amount of worker threads to create
 * TODO: Move to a config file
 */
const int gc_numWorkers = 3;
const char gc_version[] = "1.0.0.0";
const char gc_deprecatedInterfaceMessage[] = "Usage of RVExtension interface is not supported, please use callExtension array syntax";

enum StatusCodes
{
	CALL_OK = 0,
	JOB_COMPLETE = 0,
	JOB_INCOMPLETE = 1,
	CALL_E_GENERIC = 0xff
};

/**
 * \brief Initialize the worker threads
 */
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

/**
 * \brief Called once on module load. Can be used for Worker and subroutine initialization. Expects a string containing version number
 * \param output Version Number
 * \param outputSize 10240 bytes (As of A3 1.84)
 */
void RVExtensionVersion(char * output, int outputSize)
{
	InitializeWorkers();

	strcpy_s(output, outputSize, gc_version);
}

/**
 * \brief This interface is old and doesn't support arrays, parsing from this is a nightmare. Do not use
 * \param output Function "return"
 * \param outputSize 10240 bytes (As of A3 1.84)
 * \param function The string that was given to callExtesion during call
 */
void RVExtension(char * output, int outputSize, const char * function)
{
	strcpy_s(output, outputSize, gc_deprecatedInterfaceMessage); //Just stop
}

/**
 * \brief Current and actually useful interface. Provides Function name and an array of arguments (which are all char*). SQF Array and Strings are surrounded in quotes, while Number, Boolean and Object are not. SQF arrays are Valid JSON arrays.
 * \param output Output string, returned to SQF as part of return array
 * \param outputSize 10240 bytes (As of A3 1.84)
 * \param function A string that can contain function names
 * \param args Array of char* AFTER the function string
 * \param argsCnt Total amount of char*
 * \return Status code for SQF (part of the return array)
 */
int RVExtensionArgs(char * output, int outputSize, const char * function, const char ** args, int argsCnt)
{
	const std::string func = function;
	std::vector<std::string> vecArgs(args, std::next(args, argsCnt)); //use <algorithm> to convert C Style array arguments into std::string vector
	if (func == "GETRequest") {
		auto job = new ozk::GETRequest(vecArgs);
		auto id = ozk::Scheduler::GetInstance()->AddJob(job);
		return id;
	} 
	if (func == "CheckJob") {
		auto id = std::stoi(vecArgs[0]); //TODO: Sanitization
		if (ozk::Scheduler::GetInstance()->GetCompletedJob(id)) {
			return JOB_COMPLETE;
		} else {
			return JOB_INCOMPLETE;
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
	return CALL_E_GENERIC;
}
