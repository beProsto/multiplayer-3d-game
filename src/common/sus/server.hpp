#pragma once

#include "networking.hpp"

namespace SUS {

class Server: public Internal::EventPoller {
public:
	Server(const std::string& _port = SUS_DEFAULT_PORT) {
		m_Port = _port;

		m_Init = new Internal::Initialiser();
		m_TCPConnection = new Internal::Connection();
		m_UDPConnection = new Internal::Connection();

		m_TCPConnection->Create(nullptr, m_Port.c_str(), Internal::Connection::Type::TCP);
		m_UDPConnection->Create(nullptr, m_Port.c_str(), Internal::Connection::Type::UDP);

		m_TCPConnection->MakeNonBlocking();
		m_UDPConnection->MakeNonBlocking();

		m_TCPConnection->MakeListen();
		m_UDPConnection->MakeListen();

		SUS_DEB("Sockets opened\n");
	}
	~Server() {
		delete m_TCPConnection;
		delete m_UDPConnection;
		delete m_Init;
	}
	
	void Update() {
		SUS_DEB("Update:\n");
		UpdateEvents();
		UpdateClients();
		UpdateData();
	}

	DEFINE_SEND_METHODS();
	void Send(const Internal::Data& _data, Protocol _prot = Protocol::TCP) {
		Internal::DataSerialiser serialised(_data);
		if(_prot == Protocol::TCP) {
			for(uint32_t i = 0; i < m_Clients.size(); i++) {
				send(m_Clients[i].TCP, (const char*)serialised.GetData(), 4 + _data.Size, 0);
			}
		}
		else {
			for(auto& udp : m_UDPMap) {
				sendto(
					m_UDPConnection->GetSocket(), 
					(const char*)serialised.GetData(), 4 + _data.Size,
					0, (sockaddr*)&udp.second.UDP,
					(int)m_UDPConnection->GetAddrInfo()->ai_addrlen
				);
			}
		}
	}
	template <typename T> void SendTo(const T& _data, Internal::ClientID _client, Protocol _prot = Protocol::TCP) { Internal::Data data; data.Size = sizeof(T); data.Data = (void*)&_data; SendTo(data, _client, _prot); }
	void SendTo(uint32_t _size, void* _data, Internal::ClientID _client, Protocol _prot = Protocol::TCP) { Internal::Data data; data.Size = _size; data.Data = _data; SendTo(data, _client, _prot); }
	void SendTo(const Internal::Data& _data, Internal::ClientID _client, Protocol _prot = Protocol::TCP) {
		Internal::DataSerialiser serialised(_data);
		if(_prot == Protocol::TCP) {
			send(_client, (const char*)serialised.GetData(), 4 + _data.Size, 0);
		}
		else {
			sendto(
				m_UDPConnection->GetSocket(), 
				(const char*)serialised.GetData(), 4 + _data.Size,
				0, (sockaddr*)&m_TCPToUDPMap[_client],
				(int)m_UDPConnection->GetAddrInfo()->ai_addrlen
			);
		}
	}

protected:
	void UpdateClients() {
		Internal::ClientID clientSocket;
		while(true) {
			sockaddr_in clientAddr = {};
			int addrlen = sizeof(clientAddr);
			clientSocket = accept(m_TCPConnection->GetSocket(), (sockaddr*)&clientAddr, &addrlen);
			if(clientSocket == INVALID_SOCKET) {
				break;
			}

			m_TCPAddrMap[clientSocket] = clientAddr;

			SUS_DEB("Client connected: %d // PORT: %d // ADDR: %d\n", (int)clientSocket, (int)clientAddr.sin_port, (int)clientAddr.sin_addr.s_addr);
			Internal::Socket newClient = {};
			newClient.TCP = clientSocket;
			m_Clients.push_back(newClient);

			// First TCP Message contains the client's Internal::ClientID, it sends it back using UDP
			const size_t MSG_SIZE = sizeof(uint32_t) + sizeof(Internal::ClientID);
			char msg[MSG_SIZE] = {};
			*(uint32_t*)(msg)  = sizeof(Internal::ClientID);
			*(Internal::ClientID*)(msg+sizeof(uint32_t)) = clientSocket;
			send(clientSocket, msg, MSG_SIZE, 0);

			Event event;
			event.Type = EventType::ClientConnected;
			event.Client.Id = clientSocket;
			event.Client.Protocol = Protocol::TCP;
			m_Events.push(event);
		}
	}
	void UpdateData() {
		const uint32_t BUFFER_LEN = 512;
		char recvbuf[BUFFER_LEN] = {};

		// UDP Parsing
		while(true) {
			sockaddr_in udpInfo = {};
			int udpInfoLen = sizeof(udpInfo);
			int udpBytesReceived = recvfrom(m_UDPConnection->GetSocket(), recvbuf, BUFFER_LEN, 0, (sockaddr*)&udpInfo, &udpInfoLen);

			if(udpBytesReceived > 0) {
				// Stringify udp client info
				std::string id(Internal::IdUDP(udpInfo));
				if(m_UDPMap.find(id) == m_UDPMap.end()) {
					SUS_DEB("New UDP Map Member!\n");
					m_UDPMap[id];
					m_UDPMap[id].UDP = udpInfo;
					m_UDPMap[id].TCP = INVALID_SOCKET; 
				}
				if(m_UDPParser.find(id) == m_UDPParser.end()) {
					m_UDPParser[id]; // initialise if not found
				}
				m_UDPParser[id].SupplyData(recvbuf, udpBytesReceived);
			}
			else {
				break;
			}
		}

		// UDP Messages
		for(auto& udp : m_UDPParser) {
			while(!udp.second.GetDataQueue().empty()) {
				Internal::Data parsed = udp.second.GetDataQueue().front();
				udp.second.GetDataQueue().pop();
				
				SUS_DEB("Received UDP Data, [(size:) %d, (first 4 bytes:) %d]\n", (int)parsed.Size, (int)*(uint32_t*)(parsed.Data));
				
				if(m_UDPMap[udp.first].TCP == INVALID_SOCKET) {
					if(parsed.Size == sizeof(Internal::ClientID)) {
						Internal::ClientID Id = *(Internal::ClientID*)(parsed.Data);
						m_UDPMap[udp.first].TCP = Id;
						m_TCPToUDPMap[Id] = m_UDPMap[udp.first].UDP;

						SUS_DEB("UDP established: %d // PORT: %d // ADDR: %d\n", (int)Id, (int)m_TCPToUDPMap[Id].sin_port, (int)m_TCPToUDPMap[Id].sin_addr.s_addr);

						if(m_TCPAddrMap[Id].sin_port == m_TCPToUDPMap[Id].sin_port) {
							SUS_DEB("ADDRESSES CHECK OUT!!!!!\n");
						}

						Event event;
						event.Type = EventType::ClientConnected;
						event.Client.Id = m_UDPMap[udp.first].TCP;
						event.Client.Protocol = Protocol::UDP;
						m_Events.push(event);
					}
				}
				else {
					Event event;
					event.Type = EventType::MessageReceived;
					event.Message.ClientId = m_UDPMap[udp.first].TCP;
					event.Message.Protocol = Protocol::UDP;
					event.Message.Size = parsed.Size;
					event.Message.Data = (uint8_t*)parsed.Data;
					m_Events.push(event);
				}
			}
		}
		

		// TCP Parsing
		for(uint32_t i = 0; i < m_Clients.size(); i++) {
			const Internal::ClientID sock = m_Clients[i].TCP;
			while(true) {
				int countOfBytesReceived = recv(sock, recvbuf, BUFFER_LEN, 0);
				int err = WSAGetLastError();
				bool errorEncountered = err == WSAECONNRESET/* || err == WSAECONNABORTED || err == WSAECONNREFUSED*/;

				if(countOfBytesReceived > 0) {
					if(m_TCPParser.find(sock) == m_TCPParser.end()) {
						m_TCPParser[sock]; // initialise if not found
					}
					m_TCPParser[sock].SupplyData(recvbuf, countOfBytesReceived);
				}
				else if(countOfBytesReceived == 0 || errorEncountered) {
					SUS_DEB("Client disconnected: %d\n", (int)sock);
					
					for(auto it = m_UDPMap.cbegin(); it != m_UDPMap.cend();) {
						if (it->second.TCP == sock) {
							m_TCPToUDPMap.erase(it->second.TCP);
							it = m_UDPMap.erase(it); // or m_UDPMap.erase(it++) / "it = m_UDPMap.erase(it)" since C++11
						}
						else {
							++it;
						}
					}

					closesocket(sock);
					m_Clients.erase(m_Clients.begin() + i);

					Event event;
					event.Type = EventType::ClientDisconnected;
					event.Client.Id = sock;
					m_Events.push(event);

					break;
				}
				else {
					break;
				}
			}
		}

		// TCP Messages
		for(auto& tcp : m_TCPParser) {
			while(!tcp.second.GetDataQueue().empty()) {
				Internal::Data parsed = tcp.second.GetDataQueue().front();
				tcp.second.GetDataQueue().pop();
				
				SUS_DEB("Received TCP Data, [(size:) %d, (first 4 bytes:) %d]\n", (int)parsed.Size, (int)*(uint32_t*)(parsed.Data));

				Event event;
				event.Type = EventType::MessageReceived;
				event.Message.ClientId = tcp.first;
				event.Message.Protocol = Protocol::TCP;
				event.Message.Size = parsed.Size;
				event.Message.Data = (uint8_t*)parsed.Data;
				m_Events.push(event);
			}
		}
	}

protected:
	std::string m_Port;

	std::vector<Internal::Socket> m_Clients;
	std::unordered_map<std::string, Internal::Socket> m_UDPMap;
	std::unordered_map<Internal::ClientID, sockaddr_in> m_TCPToUDPMap;
	
	std::unordered_map<Internal::ClientID, sockaddr_in> m_TCPAddrMap;

	std::unordered_map<Internal::ClientID,      Internal::DataParser> m_TCPParser;
	std::unordered_map<std::string, Internal::DataParser> m_UDPParser;

	Internal::Connection* m_TCPConnection;
	Internal::Connection* m_UDPConnection;
	Internal::Initialiser* m_Init;
};

}