#pragma once
#include <boost/asio.hpp>
#include <vector>


class AsioIOServicePool {
public:
	using IOService = boost::asio::io_context;
	using Work = boost::asio::io_context::work;
	using WorkPtr = std::unique_ptr<Work>;
	~AsioIOServicePool();
	AsioIOServicePool(const AsioIOServicePool&) = delete;
	AsioIOServicePool& operator=(const AsioIOServicePool&) = delete;
	boost::asio::io_context& GetIOService();
	void Stop();
	static AsioIOServicePool& GetInstance();
private:
	AsioIOServicePool(std::size_t size = std::thread::hardware_concurrency());
	std::vector<IOService> m_ioServices;
	std::vector<WorkPtr> m_works;
	std::vector<std::thread> m_threads;
	std::size_t m_nextIOService;
};