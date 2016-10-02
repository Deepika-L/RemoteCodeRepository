////////////////////////////////////////////////////////////////////////////////////
// TestExecutive.cpp - Test client that demonstrates requirements of project 4    //
// ver 1.0                                                                        //
//                                                                                //
// Application: Project 4 for CIS 687, Spring 2016                                //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015                   //
// Author:      Deepika Lakshmanan                                                //
//              dlakshma@syr.edu                                                  //
////////////////////////////////////////////////////////////////////////////////////


#include "../Sockets/Sockets.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "../HttpMessage/HttpMessage.h"
#include "../Logger/Cpp11-BlockingQueue.h"
#include "../FileSystem/FileSystem.h"
#include "../StringClient/Client.h"
#include "../XmlMsgGenerator/XmlMsgGenerator.h"
#include "../StringClient/Client.h"

using namespace Utilities;
using namespace FileSystem;


int main() {
  std::cout << "\n\n TEST EXECUTIVE FOR PROJECT 4\n\n ";
  SocketSystem ss;
  SocketListener sl(8081, Socket::IP6);
  Client c1;
  std::thread t1(
    [&]() { c1.execute(); });
  sl.start(c1);
  std::cout << "\nClient connected to server\n";
  BlockingQueue<HttpMessage>& sendQ = c1.sendrQ();
  BlockingQueue<HttpMessage>& recvQ = c1.recvrQ();
  std::vector<std::string> cmds = { "GetPackages", "CheckIn", "CheckOut", "GetPackagesChkOut", "quit" };

  try {
    //Sender loop
    for(auto cmd:cmds) {
      HttpMessage msg;
      if (cmd == "GetPackages") { std::cout << "\n\nTesting for GetPackages request\n\n"; }
      if (cmd == "CheckIn") { std::cout << "\n\nTesting for CheckIn request\n\n"; msg.addAttribute("Package-Name", "TestExec");
      msg.addAttribute("Check-in", "open");}
      if (cmd == "CheckOut") { std::cout << "\n\nTesting for CheckOut request\n\n"; msg.addAttribute("Package-Name", "TestExec_ver1");
      msg.addAttribute("Dependencies", "True");
      }
      if (cmd == "GetPackagesChkOut") { std::cout << "\n\nTesting for Get Packages for Check out request\n\n"; }
      if (cmd == "quit") { std::cout << "\n\nTesting for quit message to exit client\n\n"; }
      msg.addBody("");
      msg.addAttribute("Command", cmd);
      msg.addAttribute("ToAddr", "127.0.0.1:8080");
      msg.addAttribute("FromAddr", "127.0.0.1:8081");
      msg.addAttribute("Mode", "OneWay");
      msg.addAttribute("Package-Location", "../ClientFiles/TestExec");
     
      sendQ.enQ(msg);
      ::Sleep(2000); //To display output in correct order
    }
  }
  catch (std::exception& ) {
    std::cout << "\n\n"; }
  t1.join();
  return 0;
}