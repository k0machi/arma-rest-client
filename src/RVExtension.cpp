#include <string>
#include <vector>
#include <iterator>
#include "Scheduler.h"
#include "GetRequest.h"
#include "Worker.h"
#include "Poco/Logger.h"
#include "Poco/AutoPtr.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/Message.h"

using Poco::Logger;
using Poco::AutoPtr;
using Poco::PatternFormatter;
using Poco::FormattingChannel;
using Poco::FileChannel;
using Poco::Message;

std::vector<std::unique_ptr<ozk::Worker>> g_ExtensionWorkers;

/**
 * \brief Amount of worker threads to create
 * TODO: Move to a config file
 */
const int gc_numWorkers = 3;
const char gc_version[] = "1.0.0.0";
const char gc_deprecatedInterfaceMessage[] = "Usage of RVExtension interface is not supported, please use callExtension array syntax";
extern const char* g_pszModuleFilename;

enum StatusCodes
{
	CALL_SUCCESS = 0x0,
	CALL_E_GENERIC = 0x0f,
	JOB_COMPLETE = 0x10,
	JOB_INCOMPLETE = 0x11,
	RESULT_COMPLETE = 0x20,
	RESULT_SLICED = 0x22,
	RESULT_NOTREADY = 0x2f,
	RESULT_INVALIDINPUT = 0x30,
	DEBUG_INFO_DUMP = 0xff
};

/**
 * \brief Initialize the worker threads
 */
void InitializeWorkers() {
	if (g_ExtensionWorkers.size() != gc_numWorkers) {
		for (size_t i = 0; i < gc_numWorkers; i++)
		{
			g_ExtensionWorkers.push_back(std::make_unique<ozk::Worker>());
		}
		Logger::get("FileLogger").information("Finished initializing worker threads");
	}

}

void InitializeLogging() {
	std::stringstream filePath;
	filePath << g_pszModuleFilename << "\\rest-client.log";
	FormattingChannel* pFCFile = new FormattingChannel(new PatternFormatter("%Y-%m-%d %H:%M:%S.%c %N[%P]:%s:%q:%t"));
	pFCFile->setChannel(new FileChannel(filePath.str()));
	pFCFile->open();

	Logger& fileLogger = Logger::create("FileLogger", pFCFile, Message::PRIO_INFORMATION);
	Logger::get("FileLogger").information("Logging Initialized");
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
	try
	{
		InitializeLogging();
		InitializeWorkers();
	}
	catch (const std::exception& e)
	{
		Logger::get("FileLogger").error("Exception thrown during init: %s", e.what());
	}

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
		auto id = ozk::Scheduler::GetInstance().AddJob(job);
		Logger::get("FileLogger").information("Added new GETRequest to queue, id: %d", id);
		return id;
	} 
	if (func == "CheckJob") {
		int id;
		try
		{
			id = std::stoi(vecArgs[0]);
		}
		catch (std::exception& e)
		{
			(void)e; //warning suppression
			return RESULT_INVALIDINPUT;
		}
		if (ozk::Scheduler::GetInstance().GetCompletedJob(id)) {
			return JOB_COMPLETE;
		} else {
			return JOB_INCOMPLETE;
		}
	}
	if (func == "GetResult") {
		int id;
		try
		{
			id = std::stoi(vecArgs[0]);
		}
		catch (std::exception& e)
		{
			(void)e;
			return RESULT_INVALIDINPUT;
		}
		ozk::Job* completedJob;
		if ((completedJob = ozk::Scheduler::GetInstance().GetCompletedJob(id))) {

			auto result = completedJob->GetResult();

			const auto totalBytes = result.size();

			if (totalBytes < outputSize) {
				strcpy_s(output, outputSize, result.c_str());
				return RESULT_COMPLETE;
			}

			auto bytesRemaining = totalBytes - completedJob->GetResultOffset();
			if (bytesRemaining < outputSize) {
				strcpy_s(output, outputSize, result.substr(completedJob->GetResultOffset(), std::string::npos).c_str());
				completedJob->ResetResultOffset();
				return RESULT_COMPLETE;
			}

			strcpy_s(output, outputSize, result.substr(completedJob->GetResultOffset(), (outputSize - 1)).c_str());
			completedJob->UpdateResultOffset(outputSize - 1);
			return RESULT_SLICED;
		}
		return RESULT_NOTREADY;
	}
	if (func == "dump") {
		std::ostringstream oss;
		int element = 1;
		for (auto i : vecArgs)
		{
			oss << "Element: " << element << ": @" << i << "@";
			element++;
		}
		Logger::get("FileLogger").information(oss.str());
		return DEBUG_INFO_DUMP;
	}
	return CALL_E_GENERIC;
}
