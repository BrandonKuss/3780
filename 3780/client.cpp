#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <bits/stdc++.h>

int main(int argc, int argv[])
{
   try{
      // Replace "localhost" with the hostname
      // that you're running your server.
      ClientSocket client_socket("localhost", 30000);
      std::string reply;
      std::string formattedReply;
      // Usually in real applications, the following
      // will be put into a loop.
      try {
	 //client_socket << "Test message.";
	 client_socket >> reply;
	 int n = reply.length();
	 std::vector<char> incoming;
	 char parity;
	 std::string ackNumber;
	 /*
	 char recieved_array[n+1];
	 strcpy(recieved_array, reply.c_str());
	 std::vector<char> incoming(std::begin(recieved_array), std::end(recieved_array));
	 char parity = incoming.pop_back();
	 std::string ackNumber = incoming.pop_back();
	 */
     /*
	 for(int i = 0; i < reply.length(); i++) {
        if (i = reply.length())
            ackNumber = reply[i];
        if (i = reply.length() - 1)
            parity = reply[i];
        incoming.push_back(reply[i]);
	 }
	 */
	 client_socket << reply;
      }
      catch(SocketException&){
      }
      std::cout << "We received this response from the server:\n\"" << reply << "\"\n";;
   }
   catch(SocketException& e){
      std::cout << "Exception was caught:" << e.description() << "\n";
   }

   return 0;
}
