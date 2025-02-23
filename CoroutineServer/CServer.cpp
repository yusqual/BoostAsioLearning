#include "CServer.h"
#include "AsioIOServicePool.h"
#include <iostream>

CServer::CServer(boost::asio::io_context& io_context, short port)
	: m_io_context(io_context), m_port(port),
	m_acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
	// ��ʼ��������
	StartAccept();
}

CServer::~CServer() {
}

void CServer::ClearSession(std::string sessionId) {
	std::lock_guard<std::mutex> lock(m_mutex);
	m_sessions.erase(sessionId);
}

void CServer::HandleAccept(std::shared_ptr<CSession> new_session, const boost::system::error_code& error) {
	if (!error) {
		// ���ӳɹ�����session����ͨ��
		new_session->Start();
		// ���������Լ�uuid����map
		std::lock_guard<std::mutex> lock(m_mutex);
		m_sessions.insert({ new_session->GetUuid(), new_session });
	} else {
		std::cout << "session accept failed, error is: " << error.what() << std::endl;
	}
	// ��acceptor��������
	StartAccept();
}

void CServer::StartAccept() {
	// ȡ��һ��io_context����׼������������
	auto& io_context = AsioIOServicePool::GetInstance().GetIOService();
	// ʹ�����io_context����һ���»Ự
	std::shared_ptr<CSession> new_session = std::make_shared<CSession>(io_context, this);
	// ����������
	m_acceptor.async_accept(new_session->GetSocket(), 
		std::bind(&CServer::HandleAccept, this, new_session, std::placeholders::_1));
}
