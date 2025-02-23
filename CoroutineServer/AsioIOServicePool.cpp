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
	// �ȴ�ÿ���߳��˳���ֹͣ
	for (auto& work : m_works) {
		work.reset();	// �ͷ�work������ָ��
	}
	for (auto& t : m_threads) {
		t.join();
	}
}

AsioIOServicePool& AsioIOServicePool::GetInstance() {
	static AsioIOServicePool instance(1);	// ����c20�������̰߳�ȫ����
	return instance;
}

AsioIOServicePool::AsioIOServicePool(std::size_t size)
	: m_ioServices(size), m_works(size), m_nextIOService(0) {
	// ��ʼ��unique_ptr
	for (std::size_t i = 0; i < size; ++i) {
		m_works[i] = std::unique_ptr<Work>(new Work(m_ioServices[i]));
	}
	// �������ioservice, ��������߳�, ÿ���߳��ڲ�����ioservice
	for (std::size_t i = 0; i < size; ++i) {
		m_threads.emplace_back([this, i]() {
			m_ioServices[i].run();
			});
	}
}
