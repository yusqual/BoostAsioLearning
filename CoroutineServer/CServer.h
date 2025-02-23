#pragma once
#include <memory>
#include <map>
#include <mutex>
#include <boost/asio.hpp>
#include "CSession.h"

class CSession;

class CServer {
public:
	// 服务器监听port端口
	CServer(boost::asio::io_context& io_context, short port);
	~CServer();
	// 从map中移除session
	void ClearSession(std::string sessionId);
private:
	// 接收连接的回调函数
	void HandleAccept(std::shared_ptr<CSession>, const boost::system::error_code& error);
	// 开始接收连接
	void StartAccept();

	boost::asio::io_context& m_io_context;
	short m_port;	// 监听端口
	boost::asio::ip::tcp::acceptor m_acceptor;	// 接收连接对象
	std::map<std::string, std::shared_ptr<CSession>> m_sessions;	// 存储会话的map
	std::mutex m_mutex;
};