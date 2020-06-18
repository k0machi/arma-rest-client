#include <vector>
#include <iostream>
#include "Job.h"
#include "Scheduler.h"
#include "Worker.h"
#include <thread>
#include "GetRequest.h"

static std::vector<ozk::Worker*> g_workers = {};
static std::vector<int> g_jobs = {};

bool gCheckJobs() {
	bool jobsLeft = false;
	for (auto i = g_jobs.begin(); i != g_jobs.end(); ++i) {
		ozk::Job* job;
		if (job = ozk::Scheduler::GetInstance().GetCompletedJob(*i)) {
			std::cout << "Result for id #" << *i << ": " << job->GetResult() << std::endl;
			ozk::Scheduler::GetInstance().Dispose(job);
		} else {
			jobsLeft = true;
		}
	}
	return jobsLeft;
}

int main() {
	for (int i = 0; i < 4; i++) {
		auto worker = new ozk::Worker();
		g_workers.push_back(worker);
	}
	//for (int i = 0; i < 100; i++) {
	auto id = ozk::Scheduler::GetInstance().AddJob(new ozk::GETRequest(*new std::vector<std::string>{"https://example.org/", "GET", "[[\"Content-Type\",\"x-www-form-urlencoded\"]]", "[]" }));
		g_jobs.push_back(id);
	//}
	std::cout << "Waiting for jobs to complete..." << std::endl;
	while (gCheckJobs()) {
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(3s);
	}
	system("pause");
	return 0;
}