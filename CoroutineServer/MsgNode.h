#pragma once

#include "const.h"
#include <iostream>
#include <string>
#include <boost/asio.hpp>

class MsgNode {
public:
	MsgNode(short max_len) :m_total_len(max_len), m_cur_len(0) {
		m_data = new char[m_total_len + 1]();
		m_data[m_total_len] = '\0';
	}

	~MsgNode() {
		std::cout << "destruct MsgNode" << std::endl;
		delete[] m_data;
	}

	// �����Ϣ�ڵ�����
	void Clear() {
		::memset(m_data, 0, m_total_len);
		m_cur_len = 0;
	}

	short m_total_len;
	short m_cur_len;
	char* m_data;
};

// ���սڵ�
class RecvNode : public MsgNode {
public:
	RecvNode(short max_len, short msg_id);

	short m_msg_id;
};

// ���ͽڵ�
class SendNode : public MsgNode {
public:
	SendNode(const char* msg, short max_len, short msg_id);

	short m_msg_id;
};