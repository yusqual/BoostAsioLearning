#pragma once
#include <boost/asio.hpp>
#include "CServer.h"
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <queue>
#include <mutex>
#include <memory>
#include "MsgNode.h"

class CServer;
class CSession : public std::enable_shared_from_this<CSession> {
public:
	CSession(boost::asio::io_context& io_context, CServer* server);
	~CSession();
	inline boost::asio::ip::tcp::socket& GetSocket() { return m_socket; }
	void Start();
	void Close();
	void Send(const char* msg, short max_length, short msg_id);
	void Send(std::string msg, short msg_id);
	inline std::string& GetUuid() { return m_uuid; }

private:
	void HandleWrite(const boost::system::error_code& error, std::shared_ptr<CSession> shared_self);

	boost::asio::io_context& m_io_context;
	CServer* m_server;
	boost::asio::ip::tcp::socket m_socket;
	std::string m_uuid;
	bool m_close;
	std::mutex m_sendLock;
	std::queue<std::shared_ptr<SendNode>> m_sendQue;
	std::shared_ptr<RecvNode> m_recvMsgNode;
	std::shared_ptr<MsgNode> m_recvHeadNode;	// 用来接收消息头部内容
};

class LogicNode {
public:
	LogicNode(std::shared_ptr<CSession>, std::shared_ptr<RecvNode>);
	std::shared_ptr<CSession> m_session;
	std::shared_ptr<RecvNode> m_recvNode;
};