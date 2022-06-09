#pragma once

#include "networking.hpp"

namespace SUS {

namespace _Event {
	struct Client {
		Internal::ClientID Id;
		Protocol Protocol;
	};
	struct Message {
		Internal::ClientID ClientId;
		Protocol Protocol;
		uint32_t Size;
		uint8_t* Data;
	};
}

enum class EventType {
	ClientConnected,
	ClientDisconnected,

	MessageReceived
};

struct Event {
	EventType Type;

	union {
		_Event::Client Client;
		_Event::Message Message;
	};
};

}