#include "CSession.h"
#include "LogicSystem.h"
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

CSession::CSession(boost::asio::io_context& io_context, CServer* server)
	:m_io_context(io_context), m_server(server), m_socket(io_context), m_close(false) {

	m_uuid = boost::uuids::to_string(boost::uuids::random_generator()());
	// 头部节点复用, 直接在构造函数初始化.
	m_recvHeadNode = std::make_shared<MsgNode>(HEAD_TOTAL_LEN);
}

CSession::~CSession() {
	try {
		std::cout << "~CSession destruct." << std::endl;
		Close();
	} catch (std::exception& e) {
		std::cout << "exception is: " << e.what() << std::endl;
	}
}

void CSession::Start() {
	// 协程处理
	// 防止智能指针被意外释放
	auto shared_this = shared_from_this();
	// 开启协程接收
	boost::asio::co_spawn(m_io_context, [=]()->boost::asio::awaitable<void> {
		try {
			while (!m_close) {
				// 协程异步读
				m_recvHeadNode->Clear();
				std::size_t n = co_await boost::asio::async_read(m_socket,
					boost::asio::buffer(m_recvHeadNode->m_data, HEAD_TOTAL_LEN), boost::asio::use_awaitable);
				if (n == 0) {
					std::cout << "receive peer closed." << std::endl;
					Close();
					m_server->ClearSession(m_uuid);
					co_return;
				}
				// 获取头部msgId数据
				short msg_id = 0;
				::memcpy(&msg_id, m_recvHeadNode->m_data, HEAD_ID_LEN);
				// 转本地字节序
				msg_id = boost::asio::detail::socket_ops::network_to_host_short(msg_id);
				if (msg_id > MAX_LENGTH) { // 假设msgid大于MAXLENGTH非法
					std::cout << "invalid msg id: " << msg_id << std::endl;
					Close();
					m_server->ClearSession(m_uuid);
					co_return;
				}
				std::cout << "msg_id is: " << msg_id << std::endl;

				// 获取msg数据长度
				short msg_len = 0;
				::memcpy(&msg_len, m_recvHeadNode->m_data + HEAD_ID_LEN, HEAD_DATA_LEN);
				msg_len = boost::asio::detail::socket_ops::network_to_host_short(msg_len);
				if (msg_len > MAX_LENGTH) {
					std::cout << "invalid msg len: " << msg_len << std::endl;
					Close();
					m_server->ClearSession(m_uuid);
					co_return;
				}

				// 读消息内容
				m_recvMsgNode = std::make_shared<RecvNode>(msg_len, msg_id);

				n = co_await boost::asio::async_read(m_socket,
					boost::asio::buffer(m_recvMsgNode->m_data, m_recvMsgNode->m_total_len),
					boost::asio::use_awaitable);

				if (n == 0) {
					std::cout << "receive peer closed." << std::endl;
					Close();
					m_server->ClearSession(m_uuid);
					co_return;
				}

				//m_recvMsgNode->m_data[m_recvMsgNode->m_total_len] = '\0';
				std::cout << "receive data is: " << m_recvMsgNode->m_data << std::endl;

				// 投递给逻辑线程处理
				LogicSystem::GetInstance().PostMsgToQue(
					std::make_shared<LogicNode>(shared_from_this(), m_recvMsgNode));

			}
		} catch (std::exception& e) {
			std::cout << e.what() << std::endl;
			// 关闭当前session
			Close();
			m_server->ClearSession(m_uuid);
		}
		}, boost::asio::detached);
}

void CSession::Close() {
	m_socket.close();
	m_close = true;
}

void CSession::Send(const char* msg, short max_length, short msg_id) {
	std::unique_lock<std::mutex> lock(m_sendLock);
	int sendQueSize = m_sendQue.size();
	if (sendQueSize > MAX_SENDQUE) {
		std::cout << "session: " << m_uuid << " send que fulled." << std::endl;
		return;
	}

	m_sendQue.push(std::make_shared<SendNode>(msg, max_length, msg_id));
	if (sendQueSize > 1) {
		return;
	}

	auto msgNode = m_sendQue.front();
	lock.unlock();
	boost::asio::async_write(m_socket, boost::asio::buffer(msgNode->m_data, msgNode->m_total_len),
		std::bind(&CSession::HandleWrite, this, std::placeholders::_1, shared_from_this()));

}

void CSession::Send(std::string msg, short msg_id) {
	Send(msg.c_str(), msg.size(), msg_id);
}

void CSession::HandleWrite(const boost::system::error_code& error, std::shared_ptr<CSession> shared_self) {
	try {
		if (!error) {
			std::unique_lock<std::mutex> lock(m_sendLock);
			m_sendQue.pop();
			if (!m_sendQue.empty()) {
				auto msgNode = m_sendQue.front();
				lock.unlock();
				boost::asio::async_write(m_socket, boost::asio::buffer(msgNode->m_data, msgNode->m_total_len),
					std::bind(&CSession::HandleWrite, this, std::placeholders::_1, shared_from_this()));
			}

		} else {
			std::cout << "handle write failed, err is: " << error.message() << std::endl;
			Close();
			m_server->ClearSession(m_uuid);
		}
	} catch (std::exception& e) {
		std::cout << "exception is: " << e.what() << std::endl;
		Close();
		m_server->ClearSession(m_uuid);
	}
}

LogicNode::LogicNode(std::shared_ptr<CSession> session, std::shared_ptr<RecvNode> recvNode)
	: m_session(session), m_recvNode(recvNode) {
}
