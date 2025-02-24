#include "LogicSystem.h"

LogicSystem::~LogicSystem() {
	m_stop = true;
	m_consume.notify_one();	// 唤醒工作线程, 待其结束任务后退出
	m_workThread.join();
}

void LogicSystem::PostMsgToQue(std::shared_ptr<LogicNode> msg) {
	std::unique_lock<std::mutex> ulock(m_mutex);
	m_msgQue.push(msg);
	if (m_msgQue.size() == 1) m_consume.notify_one();
}

LogicSystem& LogicSystem::GetInstance() {
	static LogicSystem instance;
	return instance;
}

LogicSystem::LogicSystem() : m_stop(false) {
	RegisterCallbacks();
	m_workThread = std::thread(&LogicSystem::DealMsg, this);
}

void LogicSystem::DealMsg() {
	while (true) {
		std::unique_lock<std::mutex> ulock(m_mutex);
		while (m_msgQue.empty() && !m_stop) {
			m_consume.wait(ulock);	// 待处理消息为空且未停止, 则挂起线程
		}

		if (m_stop) {
			// 即将停止服务, 处理完消息队列中的消息再退出
			while (!m_msgQue.empty()) {
				auto msgNode = m_msgQue.front();
				std::cout << "recv msg id is: " << msgNode->m_recvNode->m_msg_id << std::endl;
				auto callbackIter = m_funCallbacks.find(msgNode->m_recvNode->m_msg_id);
				if (callbackIter == m_funCallbacks.end()) {
					m_msgQue.pop();
					continue;
				}

				callbackIter->second(msgNode->m_session,
					msgNode->m_recvNode->m_msg_id,
					std::string(msgNode->m_recvNode->m_data, msgNode->m_recvNode->m_total_len));
				m_msgQue.pop();
			}
			break;
		}

		// 没有停止且队列中有数据
		auto msgNode = m_msgQue.front();
		std::cout << "recv msg id is: " << msgNode->m_recvNode->m_msg_id << std::endl;
		auto callbackIter = m_funCallbacks.find(msgNode->m_recvNode->m_msg_id);
		if (callbackIter == m_funCallbacks.end()) {
			m_msgQue.pop();
			continue;
		}

		callbackIter->second(msgNode->m_session,
			msgNode->m_recvNode->m_msg_id,
			std::string(msgNode->m_recvNode->m_data, msgNode->m_recvNode->m_total_len));
		m_msgQue.pop();
	}
}

void LogicSystem::RegisterCallbacks() {
	m_funCallbacks[MSG_HELLO_WORLD] = std::bind(&LogicSystem::HelloWorldCallback, this,
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

void LogicSystem::HelloWorldCallback(std::shared_ptr<CSession> session, const short& msg_id, const std::string& msg_data) {
	Json::Reader reader;
	Json::Value root;
	reader.parse(msg_data, root);
	std::cout << "receive msg id is: " << root["id"].asInt() << "msg data is: "
		<< root["data"].asString() << std::endl;

	root["data"] = "Server has received msg, msg data is: " + root["data"].asString();

	std::string returnStr = root.toStyledString();
	session->Send(returnStr, root["id"].asInt());
}
