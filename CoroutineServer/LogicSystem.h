#pragma once
#include <queue>
#include <thread>
#include <map>
#include <functional>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include "CSession.h"
#include "const.h"

typedef std::function<void(std::shared_ptr<CSession> session, const short& msg_id, const std::string& msg_data)> FunCallback;

class LogicSystem {
public:
	~LogicSystem();
	void PostMsgToQue(std::shared_ptr<LogicNode> msg);
	static LogicSystem& GetInstance();
	LogicSystem(const LogicSystem&) = delete;
	LogicSystem& operator=(const LogicSystem&) = delete;

private:
	LogicSystem();
	void DealMsg();
	void RegisterCallbacks();	// 注册回调函数, 把消息和回调函数绑到一起
	void HelloWorldCallback(std::shared_ptr<CSession> session, const short& msg_id, const std::string& msg_data);

	std::thread m_workThread;
	std::queue<std::shared_ptr<LogicNode>> m_msgQue;
	std::mutex m_mutex;
	std::condition_variable m_consume;
	bool m_stop;
	std::map<short, FunCallback> m_funCallbacks;
};