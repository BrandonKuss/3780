#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
#include <cstdlib>
#include <ctime>

int checkParity(std::string incoming) {
    std::string parityCheck;
    parityCheck = incoming;
    int currentCount = 0, temp;
    for (int i = 0; i < parityCheck.length(); i++) {
        char tmp = parityCheck[i];
        int translation = static_cast<int>(tmp);
        currentCount += translation;
    }
    while(currentCount != 0 ) {
        temp = currentCount % 2;
        currentCount /= 2;
    }
    if ((temp % 2) == 0)
        return 1;
    else
        return 0;
}

int getSeqNum(int current) {
    int c = current;
    if ((c%2) == 0)
        return 0;
    else
        return 1;
}

void seedRandomGenerator() {
    srand(time(0));
}

int main(int argc, int argv[])
{
   std::cout << "running..\n";
   try{
      // Create the socket
      ServerSocket server(30000);

      while (true){
	 ServerSocket new_sock;
	 server.accept(new_sock);

     //ServerSocket data_sock;
     //server.accept(data_sock);


     std::vector<std::string> toSend;
     std::string currentLine;
     std::ifstream infile;
     infile.open("s1.txt");
     while(std::getline(infile, currentLine, '.')) {
        toSend.push_back(currentLine);
     }
     bool canSend = true;
     int counter = 0;
     int seqNum = 0;
     int parity_bit;
     seedRandomGenerator();


	 // For multiple threading, you need to create
	 // a new thread here and pass new_sock to it.
	 // The thread will use new_sock to communicate
	 // with the client.
	 try{
	    while (true){
	       std::string data;
	       if (canSend) {
            std::string sending = toSend[counter];
            seqNum = getSeqNum(counter);
            sending.append(std::to_string(seqNum));
            parity_bit = checkParity(toSend[counter]);
            int rng = rand() % 5;
            if (rng = 2)
                parity_bit = (parity_bit + 1) % 2;
            sending.append(std::to_string(parity_bit));
            new_sock << sending;
	       }
	       new_sock >> data;
	       std::string reply = data;
	       if ((reply == std::to_string(seqNum)) == 0) {
                canSend = true;
                counter ++;
	       }
	       else {
            canSend = true;
	       }
	       continue;
	       //new_sock << data;
	    }
	 }
	 catch(SocketException&){
	 }
      }
   }
   catch (SocketException& e){
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
   }
  return 0;
}
