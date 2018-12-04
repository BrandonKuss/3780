#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
#include <cstdlib>
#include <ctime>


std::string checkParity(std::string incoming)
{
    std::string parityCheck;
    parityCheck = incoming;
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

std::string getSeqNum(int current)
{
    int c = current;
    if ((c%2) == 0)
        return "0";
    else
        return "1";
}

void seedRandomGenerator()
{
    srand(time(0));
}

int main(int argc, int argv[])
{
    std::cout << "running..\n";
    try
    {
        // Create the socket
        ServerSocket server(30000);
        ServerSocket sender(31000);

        while (true)
        {
            ServerSocket listening_sock;
            server.accept(listening_sock);

            ServerSocket data_sock;
            sender.accept(data_sock);


            std::vector<std::string> toSend;
            std::string currentLine;
            std::ifstream infile;
            infile.open("s1.txt");
            while(std::getline(infile, currentLine, '.'))
            {
                toSend.push_back(currentLine);
            }
            bool canSend = true;
            int i = 0;
            std::string seqNum;
            std::string reply;
            std::string parity_bit;
            std::string data;
            seedRandomGenerator();


            // For multiple threading, you need to create
            // a new thread here and pass new_sock to it.
            // The thread will use new_sock to communicate
            // with the client.
            try
            {
                while (true)
                {
                    for (i = 0; i < toSend.size(); i++)
                    {
                        if (i == toSend.size()-1){
                               std::string eof;
                               eof = "EOF";
                               data_sock << eof;
                               break;
                            }
                        do
                        {
                            std::string sending = toSend[i];
                            seqNum = getSeqNum(i);
                            parity_bit = checkParity(toSend[i]);
                            sending.append(seqNum);
                            int rng = rand() % 5;
                            if (rng == 1)
                            {
                                if (parity_bit == "1")
                                    parity_bit = checkParity("0");
                                else
                                    parity_bit = checkParity("1");
                            }
                            sending.append(parity_bit);
                            data_sock << sending;
                            listening_sock >> data;
                            std::string reply = data;
                        }
                        while (data != seqNum);
                    }

                }
            }


            catch(SocketException&)
            {
            }
        }
    }
    catch (SocketException& e)
    {
        std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
    }
    return 0;
}
