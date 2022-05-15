#define SUS_DEBUG_OFF
#include "../common/sus/networking.hpp"
#include "../common/math/math.hpp"

#define SUS_CAST_DATA(t, d) *(t*)d

#include <thread>
#include <chrono>

struct PositionalMessage {
	SOCKET Id;
	Math::Vec3 Position;
	Math::Vec3 Rotation;
};

int main(int argc, char** argv) {
	SUS::Server server;

	int count = 0;

	while(true) {
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(16ms);

		SUS::Event event;

		server.Update();

		std::unordered_map<SOCKET, Math::Vec3> playersPositions;

		while(server.PollEvent(event)) {
			switch(event.Type) {
				case SUS::EventType::ClientConnected: {
					std::cout << "Ayo new guy " << event.Client.Id << "\n";
				} break;

				case SUS::EventType::ClientDisconnected: {
					std::cout << "Guy " << event.Client.Id << " left :pensive:" << "\n";
				} break;

				case SUS::EventType::MessageReceived: {
					if(event.Message.Protocol == SUS::Protocol::UDP) {
						Math::Vec3 position = SUS_CAST_DATA(Math::Vec3, event.Message.Data);
						Math::Vec3 rotation = SUS_CAST_DATA(Math::Vec3, event.Message.Data+sizeof(Math::Vec3));
						
						std::cout << event.Message.ClientId << " [ " << position.x << ", " << position.y << ", " << position.z <<  " ]\n";
						
						playersPositions[event.Message.ClientId] = position;
						
						PositionalMessage pm;
						pm.Id = event.Message.ClientId;
						pm.Position = position;
						pm.Rotation = rotation;
						
						server.Send(pm, SUS::Protocol::UDP);
					}
				} break;
			}
		}
		
		count++;
	}
}