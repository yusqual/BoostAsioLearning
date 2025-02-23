#include "AsioIOServicePool.h"
#include <iostream>

AsioIOServicePool::~AsioIOServicePool() {
	std::cout << "AsioIOService Pool destruct" << std::endl;
}

boost::asio::io_context& AsioIOServicePool::GetIOService() {
	auto& service = m_ioServices[m_nextIOService++];
	if (m_nextIOService == m_ioServices.size()) m_nextIOService = 0;
	return service;
}

void AsioIOServicePool::Stop() {
	// 等待每个线程退出再停止
	for (auto& work : m_works) {
		work.reset();	// 释放work的智能指针
	}
	for (auto& t : m_threads) {
		t.join();
	}
}

AsioIOServicePool& AsioIOServicePool::GetInstance() {
	static AsioIOServicePool instance(1);	// 这里c20不存在线程安全问题
	return instance;
}

AsioIOServicePool::AsioIOServicePool(std::size_t size)
	: m_ioServices(size), m_works(size), m_nextIOService(0) {
	// 初始化unique_ptr
	for (std::size_t i = 0; i < size; ++i) {
		m_works[i] = std::unique_ptr<Work>(new Work(m_ioServices[i]));
	}
	// 遍历多个ioservice, 创建多个线程, 每个线程内部启动ioservice
	for (std::size_t i = 0; i < size; ++i) {
		m_threads.emplace_back([this, i]() {
			m_ioServices[i].run();
			});
	}
}
