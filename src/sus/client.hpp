#pragma once

#include "networking.hpp"
#include "internal.hpp"

#define SUSDEB(...) printf(__VA_ARGS__)

namespace SUS {

class Client: public Internal::EventPoller {
public:
	Client(const std::string& _host = "127.0.0.1", const std::string& _port = SUS_DEFAULT_PORT) {
		m_Host = _host;
		m_Port = _port;
		
		m_ID = INVALID_SOCKET;
		m_Connected = false;

		m_Init = new Internal::Initialiser();

		m_TCPConnection = new Internal::Connection();
		m_UDPConnection = new Internal::Connection();

		m_TCPConnection->Create(m_Host.c_str(), m_Port.c_str(), Internal::Connection::Type::TCP);
		m_UDPConnection->Create(m_Host.c_str(), m_Port.c_str(), Internal::Connection::Type::UDP);

		SUSDEB("Socket opened\n");
	}
	~Client() {
		delete m_UDPConnection;
		delete m_TCPConnection;
		delete m_Init;
	}

	bool Connect() {
		if(connect(m_TCPConnection->GetSocket(), m_TCPConnection->GetAddrInfo()->ai_addr, (int)m_TCPConnection->GetAddrInfo()->ai_addrlen) == SOCKET_ERROR) {
			SUSDEB("Couldn't connect to the socket!\n");
			return false;
		}
		else {
			SUSDEB("Socket connected\n");
			m_TCPConnection->MakeNonBlocking();
			m_UDPConnection->MakeNonBlocking();
			m_Connected = true;
			return true;
		}
	}
	bool IsConnected() const {
		return m_Connected;
	}

	void Disconnect() {
		m_Connected = false;
	}

	void Update() {
		SUSDEB("Update:\n");
		UpdateEvents();
		UpdateData();
	}

	DEFINE_SEND_METHODS();
	void Send(const Internal::Data& _data, Protocol _prot = Protocol::TCP) {
		Internal::DataSerialiser serialised(_data);
		if(_prot == Protocol::TCP) {
			send(m_TCPConnection->GetSocket(), (const char*)serialised.GetData(), 4 + _data.Size, 0);
		}
		else {
			sendto(
				m_UDPConnection->GetSocket(), 
				(const char*)serialised.GetData(), 4 + _data.Size,
				0, m_UDPConnection->GetAddrInfo()->ai_addr, 
				(int)m_UDPConnection->GetAddrInfo()->ai_addrlen
			); 
		}
	}

protected:
	void UpdateData() {
		const uint32_t BUFFER_LEN = 512;
		char recvbuf[BUFFER_LEN] = {};

		// UDP Messages
		while(true) {
			sockaddr_in udpInfo = {};
			int udpInfoLen = sizeof(udpInfo);
			int udpBytesReceived = recvfrom(m_UDPConnection->GetSocket(), recvbuf, BUFFER_LEN, 0, (sockaddr*)&udpInfo, &udpInfoLen);

			if(udpBytesReceived > 0) {
				uint32_t size = *(uint32_t*)recvbuf;
				uint8_t* data = (uint8_t*)(recvbuf+4);

				SUSDEB("Received UDP Data, [%d:'%d']\n", (int)udpBytesReceived, (int)*(uint32_t*)(recvbuf));

				Event event;
				event.Type = EventType::MessageReceived;
				event.Message.ClientId = 0;
				event.Message.Protocol = Protocol::UDP;
				event.Message.Size = size;
				event.Message.Data = malloc(size);
				memcpy(event.Message.Data, data, size);
				m_Events.push(event);
			}
			else {
				break;
			}
		}

		// TCP Messages
		int countOfBytesReceived = recv(m_TCPConnection->GetSocket(), recvbuf, BUFFER_LEN, 0);
		bool errorEncountered = WSAGetLastError() == WSAECONNRESET;

		// First TCP Message contains the client's SOCKET, it sends it back using UDP
		const size_t FIRST_MSG_SIZE = sizeof(uint32_t) + sizeof(SOCKET);
		if(m_ID == INVALID_SOCKET && countOfBytesReceived == FIRST_MSG_SIZE) {
			m_ID = *(SOCKET*)(recvbuf+sizeof(uint32_t));
			char msg[FIRST_MSG_SIZE] = {};
			*(uint32_t*)(msg) = sizeof(SOCKET);
			*(SOCKET*)(msg+sizeof(uint32_t)) = m_ID;
			sendto(
				m_UDPConnection->GetSocket(), 
				(const char*)msg, FIRST_MSG_SIZE,
				0, m_UDPConnection->GetAddrInfo()->ai_addr, 
				(int)m_UDPConnection->GetAddrInfo()->ai_addrlen
			);
			SUSDEB("Ping-Ponged Socket UDP Data, [%d:'%d']\n", (int)FIRST_MSG_SIZE, (int)m_ID);
		}
		else if(countOfBytesReceived > 0) {
			SUSDEB("Received TCP Data, [%d:'%d']\n", (int)countOfBytesReceived, (int)*(uint32_t*)(recvbuf));
		
			uint32_t size = *(uint32_t*)recvbuf;
			uint8_t* data = (uint8_t*)(recvbuf+4);

			Event event;
			event.Type = EventType::MessageReceived;
			event.Message.ClientId = 0;
			event.Message.Protocol = Protocol::TCP;
			event.Message.Size = size;
			event.Message.Data = malloc(size);
			memcpy(event.Message.Data, data, size);
			m_Events.push(event);
		}
		else if(errorEncountered) {
			SUSDEB("Socket disconnected!\n");
			m_Connected = false;
		}
	}

protected:
	std::string m_Host;
	std::string m_Port;

	bool m_Connected;

	SOCKET m_ID;

	// Internal::DataParser m_TCPParser;
	// Internal::DataParser m_UDPParser;

	Internal::Connection* m_TCPConnection;
	Internal::Connection* m_UDPConnection;
	Internal::Initialiser* m_Init;
};

}