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

void InitializeLogging() {
	FormattingChannel* pFCFile = new FormattingChannel(new PatternFormatter("%Y-%m-%d %H:%M:%S.%c %N[%P]:%s:%q:%t"));
	pFCFile->setChannel(new FileChannel("rest-client.log"));
	pFCFile->open();

	Logger& fileLogger = Logger::create("FileLogger", pFCFile, Message::PRIO_INFORMATION);
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
	InitializeLogging();
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
		Logger::get("FileLogger").information("Added new GETRequest to queue, id: %d", id);
		return id;
	} 
	if (func == "CheckJob") {
		auto id = std::stoi(vecArgs[0]);
		if (ozk::Scheduler::GetInstance()->GetCompletedJob(id)) {
			return 1;
		} else {
			return 0;
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
	if (func == "dump") {
		std::ostringstream oss;
		int element = 1;
		for (auto i : vecArgs)
		{
			oss << "Element: " << element << ": @" << i << "@";
			element++;
		}
		Logger::get("FileLogger").information(oss.str());

	}
	return 0xff;
}
