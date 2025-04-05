#include "Server.h"


int main()
{
    try {
        // Create the server and start listening on port 53000
        Server server(53000);
        server.Listener();

    }
    catch (const std::exception& e) {
        std::cerr << "Exception in server: " << e.what() << std::endl;
    }

    return 0;
}