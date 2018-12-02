#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <bits/stdc++.h>

int checkParity(std::string incoming)
{
    std::string parityCheck;
    parityCheck = incoming;
    int currentCount = 0, temp;
    for (int i = 0; i < parityCheck.length(); i++)
    {
        char tmp = parityCheck[i];
        int translation = static_cast<int>(tmp);
        currentCount += translation;
    }
    while(currentCount != 0 )
    {
        temp = currentCount % 2;
        currentCount /= 2;
    }
    if ((temp % 2) == 0)
        return 1;
    else
        return 0;
}

int main(int argc, int argv[])
{
    try
    {
        // Replace "localhost" with the hostname
        // that you're running your server.
        ClientSocket client_socket("localhost", 30000);
        ClientSocket listening_socket("localhost", 31000);
        std::string reply;
        std::string formattedReply;
        // Usually in real applications, the following
        // will be put into a loop.
        while (true)
        {
            try
            {
                //client_socket << "Test message.";

                listening_socket >> reply;
                int n = reply.length();
                std::string incoming;
                std::string parity;
                std::string ackNumber;
                incoming = reply.substr(0, n-2);
                formattedReply = incoming;
                parity = reply.substr(n-1, 1);
                ackNumber = reply.substr(n-2,1);
                int parityCheck = checkParity(formattedReply);
                std::cout << ackNumber;
                if (std::to_string(parityCheck) == parity)
                {
                    client_socket << ackNumber;
                }
                else
                {
                    client_socket << "";
                }
                //client_socket << ackNumber;
            }
            catch(SocketException&)
            {
            }
            std::cout << "We received this response from the server:\n\"" << formattedReply << "\"\n";
        }
    }
    catch(SocketException& e)
    {
        std::cout << "Exception was caught:" << e.description() << "\n";
    }

    return 0;
}
