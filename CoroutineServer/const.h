#pragma once
#define MAX_LENGTH 1024*2	// ��������󳤶�

#define HEAD_TOTAL_LEN 4	// ͷ���ܳ���
#define HEAD_ID_LEN 2		// ͷ��id����
#define HEAD_DATA_LEN 2		// ͷ�����ݳ���

#define MAX_RECVQUE 10000	// ���ն��г���
#define MAX_SENDQUE 1000	// ���Ͷ���

// ��Ϣid
enum MSG_ID {
	MSG_HELLO_WORLD = 1001
};