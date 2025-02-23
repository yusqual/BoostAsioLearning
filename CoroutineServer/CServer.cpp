#include "CServer.h"
#include "AsioIOServicePool.h"
#include <iostream>

CServer::CServer(boost::asio::io_context& io_context, short port)
	: m_io_context(io_context), m_port(port),
	m_acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
	// 开始监听连接
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
		// 连接成功后让session处理通话
		new_session->Start();
		// 将新连接以及uuid加入map
		std::lock_guard<std::mutex> lock(m_mutex);
		m_sessions.insert({ new_session->GetUuid(), new_session });
	} else {
		std::cout << "session accept failed, error is: " << error.what() << std::endl;
	}
	// 让acceptor继续接收
	StartAccept();
}

void CServer::StartAccept() {
	// 取出一个io_context用来准备接收新连接
	auto& io_context = AsioIOServicePool::GetInstance().GetIOService();
	// 使用这个io_context创建一个新会话
	std::shared_ptr<CSession> new_session = std::make_shared<CSession>(io_context, this);
	// 监听新连接
	m_acceptor.async_accept(new_session->GetSocket(), 
		std::bind(&CServer::HandleAccept, this, new_session, std::placeholders::_1));
}
