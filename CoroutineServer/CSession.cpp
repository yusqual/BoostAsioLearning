#include "CSession.h"
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

CSession::CSession(boost::asio::io_context& io_context, CServer* server)
	:m_io_context(io_context), m_server(server), m_socket(io_context) {
	m_uuid = boost::uuids::to_string(boost::uuids::random_generator()());
}

void CSession::Start() {
	// ¿ªÆôÐ­³Ì
}


