#define SUS_DEBUG_OFF
#include "../common/sus/networking.hpp"
#include "../common/math/math.hpp"

#include <thread>
#include <chrono>

struct PositionalMessage {
	SOCKET Id;
	Math::Vec3 Position;
	Math::Vec3 Rotation;
};
struct PositionalData {
	Math::Vec3 Position;
	Math::Vec3 Rotation;
};
struct DisconnectMessage {
	SOCKET Client;
};

int main(int argc, char** argv) {
	SUS::Server server;

	std::unordered_map<SOCKET, PositionalData> playerspos;

	while(true) {
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(16ms);

		SUS::Event event;

		server.Update();

		while(server.PollEvent(event)) {
			switch(event.Type) {
				case SUS::EventType::ClientConnected: {
					if(event.Client.Protocol == SUS::Protocol::TCP) {
						// A new client has connected
						std::cout << "Ayo new guy " << event.Client.Id << "\n";
					}
					else {
						// A connected client has established a UDP connection
						std::cout << "Ayo " << event.Client.Id << " is udp now" << "\n";
						
						// We send them all the players' positions
						for(auto& player : playerspos) {
							PositionalMessage pm;
							pm.Id = player.first;
							pm.Position = player.second.Position;
							pm.Rotation = player.second.Rotation;
							server.SendTo(pm, event.Client.Id, SUS::Protocol::UDP);
							std::cout << "Sent him " << player.first << " data!" << "\n";
						}

						// We append them to the player positions' list
						playerspos[event.Message.ClientId] = PositionalData{Math::Vec3(0.0f, 0.0f, 1.0f), Math::Vec3(0.0f)};

						// We send info about them to everyone
						PositionalMessage pm;
						pm.Id = event.Client.Id;
						pm.Position = Math::Vec3(0.0f, 0.0f, 1.0f);
						pm.Rotation = Math::Vec3(0.0f);
						server.Send(pm, SUS::Protocol::UDP);
					}
				} break;

				case SUS::EventType::ClientDisconnected: {
					// A client has disconnected
					std::cout << "Guy " << event.Client.Id << " left :pensive:" << "\n";
					playerspos.erase(event.Client.Id);
					// We inform the players about it
					server.Send(DisconnectMessage{event.Client.Id}, SUS::Protocol::TCP);
				} break;

				case SUS::EventType::MessageReceived: {
					if(event.Message.Protocol == SUS::Protocol::UDP && event.Message.Size == sizeof(PositionalData)) {
						// We've receiver data about a player's position
						PositionalData posdata = SUS_CAST_DATA(PositionalData, (event.Message.Data));
						
						std::cout << event.Message.ClientId << " [ " << posdata.Position.x << ", " << posdata.Position.y << ", " << posdata.Position.z << " : " << posdata.Rotation.x << ", " << posdata.Rotation.y << ", " << posdata.Rotation.z << " ]\n";
						
						// We inform other players of the player's position
						playerspos[event.Message.ClientId] = posdata;
						
						PositionalMessage pm;
						pm.Id = event.Message.ClientId;
						pm.Position = posdata.Position;
						pm.Rotation = posdata.Rotation;
						
						server.Send(pm, SUS::Protocol::UDP);
					}
				} break;
			}
		}
	}
}