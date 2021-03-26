
#include "ThreadController.h"

ThreadController::ThreadController()
	:interval_(1000)
{
}

ThreadController::~ThreadController()
{
	destroyFlag.store(true);
	if (threadWorker_.joinable())
	{
		threadWorker_.join();
	}
}

void ThreadController::start(std::function<void()> worker)
{
	if (threadWorker_.get_id() == std::thread::id())
	{
		readyStatus.store(true);
		worker_ = worker;
		threadWorker_ = std::thread(std::bind(&ThreadController::monitor, this));
	}
	else
	{
		/*restart*/
	}
}

void ThreadController::pause()
{
	/*대기상태로 진입한다.*/
	readyStatus.store(false);
}

void ThreadController::resume()
{
	setReady();
}

bool ThreadController::isWorking()
{
	return readyStatus;
}

void ThreadController::setInterval(int milliseconds)
{
	interval_ = milliseconds;
}

int ThreadController::getInterval()
{
	return interval_;
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
	if (ready == true)
	{
		return true;
	}
	else
	{
		/*소멸자 호출 시 false*/
		if (destroyFlag.load(std::memory_order_acquire) == true)
		{
			return false;
		}
		else
		{
			/*ready false일 경우 wait상태로 들어간다.*/
			waitReady();
		}
	}
	return true;
}

void ThreadController::waitReady()
{
	std::unique_lock<std::mutex> lock{ readyMutex };
	readyCondition.wait(lock, [&] {
		return readyStatus.load(std::memory_order_acquire);
		});

}

void ThreadController::monitor()
{
	while (destroyFlag.load(std::memory_order_acquire) == false)
	{
		if (isReady() == false)
		{
			break;
		}

		/*to do*/
		worker_();

		std::this_thread::sleep_for(std::chrono::milliseconds(interval_));
	}
}
