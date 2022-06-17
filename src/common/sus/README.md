## SUS for Networking

### Simple Unreliable Solution for Networking

This library is a simple, unreliable solution for networking.
I've made this in purpose of testing out how a simple networking solution could be implemented in C++.

Here's an overview of how the SUS Protocol works:

Message Structure (Both in TCP and UDP):
- First 4 bytes     - *uint32_t*    - The total `size` of the message (in bytes)
- Next 1 byte       - *uint8_t*     - The message `origin` (whether it's an internal message or one sent by the client etc)
- Next `size` bytes - *uint8_t\[\]* - The message `data` itself

Server Side Cycle:
1. Opens port `2137` (by default, at least) for both TCP and UDP connections.
2. When a client connects (TCP), the server sends them a TCP message containing their `Client ID`.
	- Message: `[ size: sizeof(ClientID), origin: 1, data: ClientID ]`
3. Upon receiving a reply from the client (a UDP message containing the exact same data), we send them an (empty) message, saying they've been successful.
	- Message: `[ size: 0, origin: 1, data: NULL ]`
4. Now the server can communicate with the client using both UDP and TCP messages.

Client Side Cycle:
1. Connects to port `2137` on the server (TCP).
2. Upon receiving the first message (TCP), we check if it's `origin` is `1`, and if so, then we send it back using the UDP channel. (We do it continously, as the UDP protocol isn't fully reliable.)
	- Message: `[ size: sizeof(ClientID), origin: 1, data: ClientID ]`
3. Upon receiving a reply from the server (a TCP message with `size` 0 and `origin` 1), we stop sending the Client ID using the UDP channel. We have established a full connection.
4. Now the server can communicate with the client using both UDP and TCP messages.
