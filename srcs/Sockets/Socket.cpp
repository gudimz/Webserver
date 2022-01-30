#include "Socket.hpp"

ft::Socket::Socket()
{
  _addrlen = sizeof(sockaddr_in);
}

void                ft::Socket::listenSocket(std::string ip, std::string port)
{
	int optval = 1;
	_address.sin_family = AF_INET;
	_address.sin_port = static_cast<unsigned short>((atoi(port.c_str()) << 8) | (atoi(port.c_str()) >> 8));
	// _address.sin_addr.s_addr = htonl(INADDR_ANY);
	_address.sin_addr.s_addr = (ip == "localhost") ? inet_addr("127.0.0.1") : inet_addr(ip.c_str());
	if (( _address.sin_addr.s_addr == INADDR_NONE && ip != "255.255.255.255"))
		throw Socket::BadAddress();
	if ((_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw Socket::SocketError();
	if ((setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int))) == -1)
		throw Socket::SocketError();
	if ((bind(_sock, reinterpret_cast<sockaddr *>(&_address), _addrlen)) == -1)
		throw Socket::SocketError();
	if ((listen(_sock, SOMAXCONN)) == -1)
		throw Socket::SocketError();
	fcntl(_sock, F_SETFL, O_NONBLOCK);

}

int                 ft::Socket::acceptSocket(sockaddr_in *addr, socklen_t *size) {
	int new_socket;
	if ((new_socket = accept(_sock, reinterpret_cast<sockaddr *>(addr), size)) < 0){
		throw Socket::SocketError();
	}
	return new_socket;
}

struct sockaddr_in  ft::Socket::getAddress() { return (_address); }
int                 ft::Socket::getSock() { return (_sock); }
int                 ft::Socket::getConnection() { return (_connection); }
socklen_t*          ft::Socket::getAddrlen() { return (&_addrlen); }
void                ft::Socket::setConnection(int con) { _connection = con; }