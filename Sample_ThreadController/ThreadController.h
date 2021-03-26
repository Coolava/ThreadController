#pragma once
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <functional>
class ThreadController
{
public:
	ThreadController();
	~ThreadController();
	void start(std::function<void()> worker);

	/*Pause thread*/
	void pause();
	void resume();
	bool isWorking();

	void setInterval(int milliseconds);
	int getInterval();
private:
	using condition = std::condition_variable;

	/*Thread handle*/
	std::thread threadWorker_;
	std::atomic<bool> destroyFlag;


	std::mutex readyMutex;
	condition readyCondition;
	std::atomic<bool> readyStatus;

	/*Call worker in monitor thread*/
	std::function<void()> worker_;

	void setReady();
	bool isReady();
	void waitReady();

	void monitor();

	/*interval milliseconds*/
	int interval_;
};

