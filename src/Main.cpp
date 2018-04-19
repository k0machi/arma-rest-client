#include <vector>
#include <iostream>
#include "Job.h"
#include "JobProcessor.h"
#include "Worker.h"
#include <Windows.h>

static std::vector<ozk::Worker*> g_workers = {};
static std::vector<int> g_jobs = {};

bool gCheckJobs() {
	for (auto i = g_jobs.begin(); i != g_jobs.end(); ++i) {
		ozk::Job* job;
		if (job = ozk::JobProcessor::GetInstance()->GetCompletedJob(*i)) {
			std::cout << "Result for id #" << *i << ": " << job->GetResult() << std::endl;
			g_jobs.erase(i);
		}
	}
	return true;
}

int main() {
	for (int i = 0; i < 4; i++) {
		auto worker = new ozk::Worker();
		g_workers.push_back(worker);
	}
	for (int i = 0; i < 100; i++) {
		auto id = ozk::JobProcessor::GetInstance()->NewJob("AAAAAAAAAA");
		g_jobs.push_back(id);
	}
	std::cout << "Waiting for jobs to complete..." << std::endl;
	while (!g_jobs.empty()) {
		gCheckJobs();
		Sleep(1000 * 10);
	}
	system("pause");
	return 0;
}