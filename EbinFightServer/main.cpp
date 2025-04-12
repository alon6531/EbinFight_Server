#include "Server.h"


// נניח שיש לך את ההגדרות של Server ו-Engine מראש

int main() {
 
    Server server(53000);
    server.Start();
       

    return 0;
}