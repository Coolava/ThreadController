
#include "ThreadController.h"
ThreadController::ThreadController()
	:interval_(1000)
{
	readyStatus = true;
}

ThreadController::~ThreadController()
{
	stop();
}

void ThreadController::start(std::function<void()> worker)
{
	if (threadWorker_.get_id() == std::thread::id())
	{
		destroyFlag.store(false);
		readyStatus.store(true);
		worker_ = worker;
		threadWorker_ = std::thread(std::bind(&ThreadController::monitor, this));
	}
	else
	{
		/*restart*/
		readyStatus.store(true);
	}
}

void ThreadController::stop()
{
	worker_ = std::function<void()>();
	destroyFlag.store(true);
	readyStatus.store(false);
	readyCondition.notify_one();

	if (threadWorker_.joinable())
	{
		threadWorker_.join();
	}
}

void ThreadController::end()
{
	worker_ = std::function<void()>();
	destroyFlag = true;
	readyStatus.store(false, std::memory_order_release);
	readyCondition.notify_one();

}

void ThreadController::pause()
{
	/*�����·� �����Ѵ�.*/
	readyStatus.store(false);
}

void ThreadController::resume()
{
	setReady();
}

bool ThreadController::isPaused()
{
	return (readyStatus == false) && (destroyFlag == false);
}

bool ThreadController::isWorking()
{
	return !destroyFlag;
}

void ThreadController::setInterval(int milliseconds)
{
	interval_ = milliseconds;
}

int ThreadController::getInterval()
{
	return interval_;
}

void ThreadController::setThreadPriority(unsigned long priority)
{
	SetThreadPriority(threadWorker_.native_handle(), priority);
}

void ThreadController::setReady()
{
	std::unique_lock<std::mutex> lock{ readyMutex };
	readyStatus.store(true);
	readyCondition.notify_one();
}

bool ThreadController::isReady()
{
	bool ready = readyStatus.load(std::memory_order_acquire);

	/*�Ҹ��� ȣ�� �� false*/
	if (destroyFlag.load(std::memory_order_acquire) == true)
	{
		return false;
	}
	else if (ready == false)
	{
		/*ready false�� ��� wait���·� ����.*/
		waitReady();
		return false;
	}

	return true;
}

void ThreadController::waitReady()
{
	std::unique_lock<std::mutex> lock{ readyMutex };
	/*readyCondition.wait(lock,  [&] {
		return readyStatus.load(std::memory_order_acquire);
		});*/

	readyCondition.wait_for(lock, std::chrono::milliseconds(interval_), [&] {
		return readyStatus.load(std::memory_order_acquire);
		});

}

void ThreadController::monitor()
{
	//unsigned int count = 1;
	//auto start = std::chrono::system_clock::now();
	while (destroyFlag.load(std::memory_order_acquire) == false)
	{
		//count++;
		if (isReady() == false)
		{
			continue;
		}


		auto start = std::chrono::system_clock::now();
		/*to do*/
		worker_();

		std::this_thread::sleep_until(start + std::chrono::milliseconds(interval_) );
	}
}
