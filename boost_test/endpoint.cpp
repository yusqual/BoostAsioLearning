#include "endpoint.h"
#include <boost/asio.hpp>
#include <iostream>

using namespace boost;

int client_endpoint() {
	std::string raw_ip_address = "";
	USHORT port_num = 3333;
	boost::system::error_code ec;
	auto ip_address = asio::ip::address::from_string(raw_ip_address, ec);
	if (ec.value() != 0) {
		std::cout << "ip address. err code = " << ec.value() << " Msg is:" << ec.message() << std::endl;
		return ec.value();
	}
	return 0;
}

int connect_to_end() {
	// Step 1. 准备服务端的ip地址和端口
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3333;
	try {
		// Step 2. 创建一个指定服务端的端点
		boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(raw_ip_address), port_num);
		boost::asio::io_context ios;
		// Step 3. 创建并打开一个socket
		boost::asio::ip::tcp::socket sock(ios, ep.protocol());
		// Step 4. 连接端点
		sock.connect(ep);
		// At this point socket 'sock' is connected to 
		// the server application and can be used
		// to send data to or receive data from it.
	} catch (system::system_error& e) { // from_string() 和 connect()如果出错会抛异常
		std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what();
		return e.code().value();
	}
}

int bind_acceptor_socket() {
	// Step 1. 这里假设服务端程序已经指定端口
	unsigned short port_num = 3333;
	// Step 2. 创建端点
	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any(), port_num);
	// Used by 'acceptor' class constructor.
	boost::asio::io_context ios;
	// Step 3. Creating and opening an acceptor socket.
	boost::asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
	boost::system::error_code ec;
	// Step 4. Binding the acceptor socket.
	acceptor.bind(ep, ec);
	// Handling errors if any.
	if (ec.value() != 0) {
		// Failed to bind the acceptor socket. Breaking
		// execution.
		std::cout << "Failed to bind the acceptor socket."
			<< "Error code = " << ec.value() << ". Message: "
			<< ec.message();
		return ec.value();
	}
	return 0;
}
