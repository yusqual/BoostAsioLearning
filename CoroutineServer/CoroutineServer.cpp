#include "CServer.h"
#include "AsioIOServicePool.h"
#include <csignal>
#include <iostream>
#include <thread>
#include <mutex>

int main() {
	try {
		auto& pool = AsioIOServicePool::GetInstance();
		boost::asio::io_context io_context;
		boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);	// ≤∂ªÒ–≈∫≈
		signals.async_wait([&io_context, &pool](auto, auto) {
			io_context.stop();
			pool.Stop();
			});

		CServer s(io_context, 10086);
		io_context.run();
	} catch (std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
	}
}