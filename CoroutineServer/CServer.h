#pragma once
#include <memory>
#include <map>
#include <mutex>
#include <boost/asio.hpp>
#include "CSession.h"

class CSession;

class CServer {
public:
	// ����������port�˿�
	CServer(boost::asio::io_context& io_context, short port);
	~CServer();
	// ��map���Ƴ�session
	void ClearSession(std::string sessionId);
private:
	// �������ӵĻص�����
	void HandleAccept(std::shared_ptr<CSession>, const boost::system::error_code& error);
	// ��ʼ��������
	void StartAccept();

	boost::asio::io_context& m_io_context;
	short m_port;	// �����˿�
	boost::asio::ip::tcp::acceptor m_acceptor;	// �������Ӷ���
	std::map<std::string, std::shared_ptr<CSession>> m_sessions;	// �洢�Ự��map
	std::mutex m_mutex;
};