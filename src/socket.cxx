//////////////////////////////////////////////////////////////////////////////////
/// ARGSS - Copyright (c) 2009 - 2010, Alejandro Marzini (vgvgf)
///         All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
///     * Redistributions of source code must retain the above copyright
///       notice, this list of conditions and the following disclaimer.
///     * Redistributions in binary form must reproduce the above copyright
///       notice, this list of conditions and the following disclaimer in the
///       documentation and/or other materials provided with the distribution.
///
/// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY
/// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
/// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
/// DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
/// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
/// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
/// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
/// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
/// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
/// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include "socket.hxx"
#include <argss/error.hxx>

#include <boost/smart_ptr.hpp>

#ifndef ARGSS_WIN32
	#define closesocket close
#endif

Socket::Socket()
{
	m_Socket = ::socket(AF_INET, SOCK_STREAM, AF_INET);
}

void Socket::Connect(const std::string& host, uint16_t port)
{
	struct sockaddr_in hostAddr;
	hostAddr.sin_family = AF_INET;
	hostAddr.sin_addr.s_addr = inet_addr(host.c_str());
	hostAddr.sin_port = htons(port);

	if ( ::connect(m_Socket, (struct sockaddr*)&hostAddr, sizeof(hostAddr)) ) {
		rb_raise(ARGSS::AError::getID(), "Could not connect to the host, make sure the host address is valid and the port is correct.");
	}
}

void Socket::Listen(uint16_t port, int maxclients)
{
	struct sockaddr_in machineAddr;
	machineAddr.sin_family = AF_INET;
	machineAddr.sin_addr.s_addr = INADDR_ANY;
	machineAddr.sin_port = htons(port);

	if (bind(m_Socket, (struct sockaddr*)&machineAddr, sizeof(machineAddr))) {
		rb_raise(ARGSS::AError::getID(), "Could not bind the socket, port specified might be used.");
		return;
	}

	if (listen(m_Socket, maxclients)) {
		rb_raise(ARGSS::AError::getID(), "Could not set the socket to listening mode, make sure the arguments are right.");
	}
}

std::vector< uint8_t > Socket::Receive()
{
	uint16_t msglen = 0;
	std::vector< uint8_t > ret;
	if (recv(m_Socket, reinterpret_cast< char* >(&msglen), sizeof(uint16_t), 0) <= 0) {
		rb_raise(ARGSS::AError::getID(), "Could not receive the message length.");
	} else {
		msglen = ntohs(msglen); // to host endian
		ret.resize(msglen);
		if (recv(m_Socket, reinterpret_cast< char* >( &(ret[0]) ), msglen, 0) <= 0) {
			rb_raise(ARGSS::AError::getID(), "Could not receive the message.");
		}
	}

	return ret;
}
void Socket::Send(std::vector< uint8_t > const& buffer)
{
	uint16_t bufferSize = buffer.size();
	boost::scoped_array< char > pFinalBuffer( new char[bufferSize + sizeof(uint16_t)] );
	// message size (to network endian)
	uint16_t msglen = htons(bufferSize);
	memcpy(pFinalBuffer.get(), &msglen, sizeof(uint16_t));
	// message body
	memcpy(pFinalBuffer.get() + sizeof(uint16_t), &(buffer[0]), bufferSize);

	if (send(m_Socket, pFinalBuffer.get(), sizeof(uint16_t) + bufferSize, 0) <= 0) {
		rb_raise(ARGSS::AError::getID(), "Could not send the message.");
	}
}

/*
Buffer Socket::Receive()
{
	Buffer buffer(0);
	uint16_t msglen = 0;
	if (recv(m_Socket, (char*)&msglen, 2, 0) <= 0) {
		rb_raise(ARGSS::AError::getID(), "Could not receive the message length.");
	} else {
		buffer = Buffer(msglen);
		char* pBufferData = buffer.getData();
		if (recv(m_Socket, pBufferData, msglen, 0) <= 0) {
			rb_raise(ARGSS::AError::getID(), "Could not receive the message.");
		}
	}

	return buffer;
}
void Socket::Send(Buffer buffer)
{
	uint16_t bufferSize = buffer.getSize();
	boost::scoped_array< char > pFinalBuffer( new char[2 + bufferSize] );
	memcpy(pFinalBuffer.get(), &bufferSize, 2);
	memcpy(pFinalBuffer.get() + 2, buffer.getData(), bufferSize);

	if (send(m_Socket, pFinalBuffer.get(), 2 + bufferSize, 0) <= 0) {
		rb_raise(ARGSS::AError::getID(), "Could not send the message.");
	}
}
 */

void Socket::Shutdown()
{
	if (m_Socket) {
	#ifdef ARGSS_WIN32
		::shutdown(m_Socket, SD_BOTH);
	#else
		::shutdown(m_Socket, SHUT_RDWR);
	#endif
	}
}

Socket::~Socket()
{
	if (m_Socket) {
		Shutdown();
		closesocket(m_Socket);
	}
}
