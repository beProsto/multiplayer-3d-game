#include "../sus/networking.hpp"

#include <thread>
#include <chrono>

int main(int argc, char** argv) {
	SUS::Server server;

	int count = 0;

	while(true) {
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(20ms);

		SUS::Event event;

		server.Update();

		while(server.PollEvent(event)) {
			switch(event.Type) {
				case SUS::EventType::ClientConnected: {
					std::cout << "Ayo new guy" << "\n";
					server.Send(32);
					int count = 0;
				} break;

				case SUS::EventType::ClientDisconnected: {
					std::cout << "Guy left :pensive:" << "\n";
					
				} break;

				case SUS::EventType::MessageReceived: {
					std::cout << "count PONG! " << count << "\n"; 
				} break;
			}
		}
		
		count++;
	}
}