#include "MsgNode.h"

RecvNode::RecvNode(short max_len, short msg_id) : MsgNode(max_len), m_msg_id(msg_id) {

}

SendNode::SendNode(const char* msg, short msg_len, short msg_id)
	: MsgNode(msg_len + HEAD_TOTAL_LEN), m_msg_id(msg_id) {
	// 存储id
	short msg_id_net = boost::asio::detail::socket_ops::host_to_network_short(msg_id);
	::memcpy(m_data, &msg_id_net, HEAD_ID_LEN);
	// 存储数据长度
	auto msg_len_net = boost::asio::detail::socket_ops::host_to_network_short(msg_len);
	::memcpy(m_data + HEAD_ID_LEN, &msg_len_net, HEAD_DATA_LEN);
	// 存储数据
	::memcpy(m_data + HEAD_TOTAL_LEN, msg, msg_len);
}