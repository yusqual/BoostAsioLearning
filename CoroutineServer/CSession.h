#pragma once
#include <boost/asio.hpp>
#include "CServer.h"

class CSession : public std::enable_shared_from_this<CSession> {
public:
	CSession(boost::asio::io_context& io_context, CServer* server);
	inline boost::asio::ip::tcp::socket& GetSocket() { return m_socket; }
	void Start();
	inline std::string& GetUuid() { return m_uuid; }

private:
	boost::asio::io_context& m_io_context;
	CServer* m_server;
	boost::asio::ip::tcp::socket m_socket;
	std::string m_uuid;
};