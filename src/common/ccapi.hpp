#pragma once
/***
 * 	This file contains the Common Communications API for our game.
 * 	The Common there means that it's going to be used both by the client (game) and the server side.
***/

namespace Comm {

class Comm {
public:
	Comm() {

	}

	~Comm() {

	}

	template<typename T>
	void Serialise(uint32_t _id, const T& _data) {

	}

protected:


};

} // namespace CCAPI