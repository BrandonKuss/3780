#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
#include <cstdlib>
#include <ctime>
#include <pthread.h>


//ServerSocket sender(31000);
ServerSocket server(30000);
ServerSocket listening_sock;

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
{server.accept(listening_sock);
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


void *threadProcess(void *ptr)
{
    ServerSocket data_sock = *((ServerSocket*)ptr);
    //ServerSocket data_sock;
    //sender.accept(data_sock);
    bool validRequest = false;

    std::vector<std::string> toSend;
    std::string currentLine;
    std::ifstream infile;
    std::string data;
    while (validRequest==false)
    {
        listening_sock >> data;
        if (data=="File1")
        {
            validRequest = true;
            infile.open("s1.txt");
        }
        else if (data=="File2")
        {
            validRequest =true;
            infile.open("s2.txt");
        }
        else
        {
            validRequest = false;
            data_sock << "Invalid request";
        }
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
                    if (i == toSend.size()-1)
                    {
                        std::string eof;
                        eof = "EOF";
                        listening_sock << eof;
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
                        listening_sock << sending;
                        listening_sock >> data;
                        std::string reply = data;
                    }
                    while (data != seqNum);
                }
            }
        }


        catch(SocketException& e)
        {
            std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
        }
    }
}




int main(int argc, int argv[])
{
    pthread_t thread1, thread2, thread3, thread4, thread5;
    int th1, th2, th3, th4, th5;
    const char *message1 = "Thread 1";
    const char *message2 = "Thread 2";
    const char *message3 = "Thread 3";
    const char *message4 = "Thread 4";
    const char *message5 = "Thread 5";
    std::cout << "running..\n";
    try
    {
        server.accept(listening_sock);
        void* p = (void*)&listening_sock;
        // Create the socket
        while (true)
        {
            th1 = pthread_create(&thread1, NULL, threadProcess, p);
            if(th1)
            {
                std::cout << "Failed to create thread";
            }

            th2 = pthread_create(&thread2, NULL, threadProcess, p);
            if(th2)
            {
                std::cout << "Failed to create thread";
            }

            th3 = pthread_create(&thread3, NULL, threadProcess, p);
            if(th3)
            {
                std::cout << "Failed to create thread";
            }

            th4 = pthread_create(&thread4, NULL, threadProcess, p);
            if(th4)
            {
                std::cout << "Failed to create thread";
            }

            th5 = pthread_create(&thread5, NULL, threadProcess, p);
            if(th5)
            {
                std::cout << "Failed to create thread";
            }


            pthread_join(thread1, NULL);
            pthread_join(thread2, NULL);
            pthread_join(thread3, NULL);
            pthread_join(thread4, NULL);
            pthread_join(thread5, NULL);

        }
    }
        catch(SocketException& e)
        {
            std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
        }
        return 0;

}
