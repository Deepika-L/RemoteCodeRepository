////////////////////////////////////////////////////////////////////////////////////
// Client.cpp - Client class that interacts with the server and GUI               //
// ver 1.0                                                                        //
//                                                                                //
// Application: Project 4 for CIS 687, Spring 2016                                //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015                   //
// Source:      Jim Fawcett, Syracuse University                                  //
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

using namespace Utilities;
using namespace FileSystem;


//Start sender of client
void Client::execute()
{
  try
  {
    SocketSystem ss;
    SocketConnecter si;
    while (!si.connect("localhost", 8080))
    {
      std::cout << "\n client waiting to connect\n\n\n";
      ::Sleep(100);
    }
    while (true) {
      HttpMessage send = sendQ.deQ();
      std::string cmd = send.findValue("Command");
      if (cmd == "quit") {
        send.addContentLength();
        std::string msg = send.buildMessage();
        std::cout << "\nSending message: \n" << msg;
        sendMessage(msg, si);
        break;
      }
      else if (cmd == "CheckIn") {
        CheckIn(send, si);
      }
      else if (cmd == "CheckOut") {
        CheckOut(send, si);
      }
      else if (cmd == "FileTransfer") {}

      send.addContentLength();
      std::string msg = send.buildMessage();
      std::cout << "\nSending message: \n" << msg;
      sendMessage(msg, si);
    }
  }
  catch (std::exception& exc) {
    std::cout << "\n  Exeception caught: ";
    std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
    std::cout << exMsg;
  }
}


//Build message for package check out
void Client::CheckOut(HttpMessage& send, Socket &si) {
  Package pkg;
  std::string txt = send.findValue("Package-Name");
  size_t pos = txt.find("ver");
  std::string ver = txt.substr(pos + 3);
  std::string name = txt.substr(0, pos - 1);
  pkg = { name, ver, "closed" };
  std::string deps = send.findValue("Dependencies");

  XmlMsgGenerator xgen;
  std::string msg = xgen.getRequestBodyForCheckOut(pkg, deps);
  send.addBody(msg);
}


//Build message for package check in
void Client::CheckIn(HttpMessage& send, Socket &si) {
  std::string pkgloc = send.findValue("Package-Location");
  std::vector<std::string> chkInFiles = Directory::getFiles(pkgloc);
  for (auto file : chkInFiles) {
    FileCheckIn(pkgloc + "/" + file, si);
  }
  Package pkg;
  pkg.name = send.findValue("Package-Name");
  pkg.status = send.findValue("Check-in");

  vector<Package> packages;

  HttpMessage::Attributes atts = send.attributes();
  for (auto att : atts) {
    if (att.first == "Dependencies") {
      Package pkg;
      std::string txt = att.second;
      size_t pos = txt.find("ver");
      std::string ver = txt.substr(pos + 3);
      std::string name = txt.substr(0, pos - 1);
      pkg = { name, ver, "closed" };
      packages.push_back(pkg);
    }
  }

  XmlMsgGenerator xgen;
  std::string body = xgen.getRequestBodyForCheckIn(pkg, packages);
  send.addBody(body);
}


//Start clienthandler to listen on port
void Client::operator()(Socket & socket)
{
  while (true)
  {
    std::string msg = socket.recvString();
    std::cout << "\nRecieved message: \n" << msg;

    HttpMessage recvd;
    recvd.parseMessage(msg);
    recvQ.enQ(recvd);

    if (recvd.findValue("Command") == "quit") {
      break;
    }

    else if (recvd.findValue("Command") == "FileTransfer")
    {
      FileCheckOut(recvd.findValue("File"), recvd.findValue("File-Length"), socket);
    }
  }
}


//Carry out file transfer for package check out
void Client::FileCheckOut(const std::string& filen, const std::string& filelength, Socket& socket) {
  std::string filename = filen;
  int fileSize = std::stoi(filelength);

  std::string name = Path::getName(filename);
  std::string fqname = "../ClientDownloads/" + name;
  FileSystem::File file(fqname);
  file.open(FileSystem::File::out, FileSystem::File::binary);
  if (!file.isGood())
  {
    /*
    * This error handling is incomplete.  The client will continue
    * to send bytes, but if the file can't be opened, then the server
    * doesn't gracefully collect and dump them as it should.  That's
    * an exercise left for students.
    */
    std::cout << "\n\n  can't open file " + fqname;
    //return false;
  }

  const size_t BlockSize = 2048;
  Socket::byte buffer[BlockSize];

  size_t bytesToRead;
  while (true)
  {
    if (fileSize > BlockSize)
      bytesToRead = BlockSize;
    else
      bytesToRead = fileSize;

    socket.recv(bytesToRead, buffer);

    FileSystem::Block blk;
    for (size_t i = 0; i < bytesToRead; ++i)
      blk.push_back(buffer[i]);

    file.putBlock(blk);
    if (fileSize < BlockSize)
      break;
    fileSize -= BlockSize;
  }
  file.close();
  //return true;
}


//Send message through socket
void Client::sendMessage(std::string& msg, Socket& socket)
{
  socket.sendString(msg);
}


//Carry out file transfer for package check in
bool Client::FileCheckIn(const std::string& filename, Socket& socket)
{
  // assumes that socket is connected
  std::string fqname = filename;
  FileSystem::FileInfo fi(fqname);
  size_t fileSize = fi.size();
  std::string sizeString = Converter<size_t>::toString(fileSize);
  FileSystem::File file(fqname);
  file.open(FileSystem::File::in, FileSystem::File::binary);
  if (!file.isGood())
    return false;

  HttpMessage msg;
  msg.addAttribute("Command", "FileTransfer");
  msg.addAttribute("ToAddr", "127.0.0.1:8080");
  msg.addAttribute("FromAddr", "127.0.0.1:8081");
  msg.addAttribute("File", filename);
  msg.addAttribute("File-Length", sizeString);
  msg.addContentLength();

  sendMessage(msg.buildMessage(), socket);
  const size_t BlockSize = 2048;
  Socket::byte buffer[BlockSize];
  while (true)
  {
    FileSystem::Block blk = file.getBlock(BlockSize);
    if (blk.size() == 0)
      break;
    for (size_t i = 0; i < blk.size(); ++i)
      buffer[i] = blk[i];
    socket.send(blk.size(), buffer);
    if (!file.isGood())
      break;
  }
  file.close();
  return true;
}


//----------------------------------------------Test stub----------------------------------------------
#ifdef TEST_CLIENT

int main()
{
  std::cout << "\n Client started\n ";

  SocketSystem ss;
  SocketListener sl(8081, Socket::IP6);
  Client c1;

  std::thread t1(
    [&]() { c1.execute(); });

  sl.start(c1);

  BlockingQueue<HttpMessage>& sendQ = c1.sendrQ();
  BlockingQueue<HttpMessage>& recvQ = c1.recvrQ();

  std::string cmd = "";
  try {
    //Sender loop
    while (cmd != "quit") {
      std::cout << "\n\n";
      //std::cout << "Enter command: ";
      std::getline(std::cin, cmd);

      HttpMessage msg;
      msg.addBody("");
      msg.addAttribute("Command", cmd);
      msg.addAttribute("ToAddr", "127.0.0.1:8080");
      msg.addAttribute("FromAddr", "127.0.0.1:8081");
      msg.addAttribute("Mode", "OneWay");
      msg.addAttribute("Package-Location", "../ClientFiles/Logger");
      msg.addAttribute("Package-Name", "Package 2_ver1");
      msg.addAttribute("Dependencies", "True");
      sendQ.enQ(msg);

      if (cmd == "quit") {
        break;
      }
    }
  }
  catch (std::exception& ) {
    std::cout << "\n\n";
  }
  t1.join();
  return 0;
}

#endif