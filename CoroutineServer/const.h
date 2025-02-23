#pragma once
#define MAX_LENGTH 1024*2	// 单个包最大长度

#define HEAD_TOTAL_LEN 4	// 头部总长度
#define HEAD_ID_LEN 2		// 头部id长度
#define HEAD_DATA_LEN 2		// 头部数据长度

#define MAX_RECVQUE 10000	// 接收队列长度
#define MAX_SENDQUE 1000	// 发送队列

// 消息id
enum MSG_ID {
	MSG_HELLO_WORLD = 1001
};