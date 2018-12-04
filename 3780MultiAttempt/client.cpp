#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <bits/stdc++.h>

std::string checkParity(std::string inc)
{
    std::string parityCheck;
    parityCheck = inc;
    int currentCount = 0, temp = 0;
    for (int i = 0; i < parityCheck.length(); i++)
    {
        char tmp = parityCheck[i];
        int translation = static_cast<int>(tmp);
        currentCount += translation;
    }
    while(currentCount != 0 )
    {
        temp += currentCount % 2;
        currentCount /= 2;
    }
    if ((temp % 2) != 0)
        return "1";
    else
        return "0";
}

int main(int argc, int argv[])
{
    try
    {
        // Replace "localhost" with the hostname
        // that you're running your server.
        ClientSocket client_socket("localhost", 30000);
        //ClientSocket listening_socket("localhost", 31000);
        std::cout << "Request File1 or File2 ";
        std::string file;
        std::cin >> file;
        client_socket << file;
        std::string reply;
        std::string formattedReply;
        // Usually in real applications, the following
        // will be put into a loop.
        while (true)
        {
            try
            {
                //client_socket << "Test message.";

                client_socket >> reply;
                if (reply == "EOF") {
                    std::cout << "Press any key to continue";
                    std::cin.clear();
                    char temp;
                    std::cin.get(temp);
                    std::cin.ignore(100, '\n');
                }
                int n = reply.length();
                std::string incoming;
                std::string parity;
                std::string ackNumber;
                incoming = reply.substr(0, n-2);
                formattedReply = incoming;
                parity = reply.substr(n-1, 1);
                ackNumber = reply.substr(n-2,1);
                std::string parityCheck;
                parityCheck = checkParity(formattedReply);
                std::cout << parity << " " << parityCheck << " " << ackNumber << " " << reply;
                if (parityCheck == parity)
                {
                    std::string ackReply = ackNumber;
                    client_socket << ackReply;
                }
                else
                {
                    client_socket << parityCheck;
                }
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
