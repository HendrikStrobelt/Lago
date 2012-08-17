#pragma comment(lib, "Ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>

#include "Client.hpp"

int main( int argc, const char* argv[] ) {

	Client client = Client("127.0.0.1", "4224");
	client.connect2Server();

	while (client.receiveString() != "getData#") {

	}
	client.sendString("exit#");

	client.disconnect();
}