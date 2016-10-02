///////////////////////////////////////////////////////////////////////////////////////////////////////
// Server.cpp - Server class that interacts with the client and carries out Repository functions     //
// ver 1.0                                                                                           //
//                                                                                                   //
// Application: Project 4 for CIS 687, Spring 2016                                                   //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015                                      //
// Source:      Jim Fawcett, Syracuse University                                                     //
// Author:      Deepika Lakshmanan                                                                   //
//              dlakshma@syr.edu                                                                     //
///////////////////////////////////////////////////////////////////////////////////////////////////////


#include <string>
#include <iostream>
#include <limits>
#include "Server.h"
#include "../Sockets/Sockets.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "../HttpMessage/HttpMessage.h"
#include "../Logger/Cpp11-BlockingQueue.h"
#include "../FileSystem/FileSystem.h"


using namespace FileSystem;
using namespace Utilities;


//Start sender of server
void Server::execute()
{
  try
  {
    SocketSystem ss;
    SocketConnecter si;
    while (!si.connect("localhost", 8081))
    {
      std::cout << "\n server waiting to connect\n\n\n";
      ::Sleep(100);
    }
    while (true) {
      HttpMessage send = sendQ.deQ();
      std::string msg = send.buildMessage();
      std::cout << "\nSending message: \n" << msg;
      sendMessage(msg, si);
      if (send.findValue("Command") == "quit")
        break;

      else if (send.findValue("Command") == "CheckOut") {
        std::vector<std::string> dloads = repo.checkOut(send);
        for (auto file : dloads) {
          FileCheckOut(file, si);
        }
      }

      else if (send.findValue("Command") == "SendFile") {
        std::vector<std::string> files = FileSystem::Directory::getFiles("../TestFiles", "*.cpp");
        sendFile(files[0], si);
      }
    }
  }
  catch (std::exception& exc)
  {
    std::cout << "\n  Exeception caught: ";
    std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
    std::cout << exMsg;
  }
}

//Start clienthandler to listen on port
void Server::operator()(Socket & socket)
{//Receiver
  while (true)
  {
    std::string msg = socket.recvString();
    std::cout << "\nRecieved message: \n" << msg;

    HttpMessage recvd;
    recvd.parseMessage(msg);
    recvQ.enQ(recvd);
    std::string cmd = recvd.findValue("Command");
    if (cmd == "quit")
      break;

    else if (cmd == "GetPackages") {
      GetPkgs();
    }

    else if (cmd == "GetPackagesChkOut") {
      GetPkgsChkOut();
    }

    else if (cmd == "CheckIn") {
      repo.checkIn(recvd);
      CheckInDone();
    }

    else if (cmd == "CheckOut") {
      HttpMessage msg;
      msg.addAttribute("Command", "CheckOut");
      msg.addAttribute("ToAddr", "127.0.0.1:8081");
      msg.addAttribute("FromAddr", "127.0.0.1:8080");
      msg.addAttribute("Mode", "OneWay");
      msg.addAttribute("Package-name", recvd.findValue("Package-name"));
      msg.addAttribute("Dependencies", recvd.findValue("Dependencies"));
      std::string body = recvd.getBody();
      msg.addBody(body);
      msg.addContentLength();

      sendQ.enQ(msg);
    }

    else if (cmd == "FileTransfer")
    {
      FileCheckIn(recvd, socket);
    }
  }
}

//Function to execute to indicate package check-in has completed
void Server::CheckInDone() {
  HttpMessage msg;

  msg.addAttribute("Command", "CheckInDone");
  msg.addAttribute("ToAddr", "127.0.0.1:8081");
  msg.addAttribute("FromAddr", "127.0.0.1:8080");
  msg.addAttribute("Mode", "OneWay");
  msg.addContentLength();
  sendQ.enQ(msg);
}

//Handles files for package check in
void Server::FileCheckIn(HttpMessage& recvd, Socket& socket) {
  std::string filename = recvd.findValue("File");
  int fileSize = std::stoi(recvd.findValue("File-Length"));

  std::string name = Path::getName(filename);
  std::string fqname = "../TestFiles/" + name;
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

//Returns list of packages currently in repository
void Server::GetPkgs() {

  std::vector<std::string> files;
  std::vector<std::string> dirs = Directory::getDirectories("../RepositoryFiles/");

  HttpMessage msg;
  msg.addAttribute("Command", "PackageList");
  msg.addAttribute("ToAddr", "127.0.0.1:8081");
  msg.addAttribute("FromAddr", "127.0.0.1:8080");
  msg.addAttribute("Mode", "OneWay");

  std::string msgbody;
  std::unordered_map<Package, vector<Package>> pkgDeps = repo.getPkgDependencies();
  for (auto it : pkgDeps) {
    std::string name = it.first.name;
    msgbody += it.first.name;
    msgbody += "_ver";
    msgbody += it.first.version;
    msgbody += "\n";
  }

  msg.addBody(msgbody);
  msg.addContentLength();

  sendQ.enQ(msg);
}

//Returns list of packages available for check-out
void Server::GetPkgsChkOut() {

  std::vector<std::string> files;
  std::vector<std::string> dirs = Directory::getDirectories("../RepositoryFiles/");

  HttpMessage msg;
  msg.addAttribute("Command", "PackageListChkOut");
  msg.addAttribute("ToAddr", "127.0.0.1:8081");
  msg.addAttribute("FromAddr", "127.0.0.1:8080");
  msg.addAttribute("Mode", "OneWay");

  std::string msgbody;

  for (auto file : dirs) {
    if (file != "." && file != "..") {
      msgbody += file;
      msgbody += "\n";
    }
  }
  msg.addBody(msgbody);
  msg.addContentLength();

  sendQ.enQ(msg);
}

//Send message through socket
void Server::sendMessage(std::string& msg, Socket& socket)
{
  socket.sendString(msg);
}

//Carry out file transfer for package check in
bool Server::sendFile(const std::string& filename, Socket& socket)
{
  // assumes that socket is connected

  std::string fqname = "../TestFiles/" + filename;

  FileSystem::FileInfo fi(fqname);
  size_t fileSize = fi.size();
  std::string sizeString = Converter<size_t>::toString(fileSize);
  FileSystem::File file(fqname);
  file.open(FileSystem::File::in, FileSystem::File::binary);
  if (!file.isGood())
    return false;

  HttpMessage msg;
  msg.addBody("");
  msg.addAttribute("POST", "Message");
  msg.addAttribute("Command", "sendfile");
  msg.addAttribute("ToAddr", "127.0.0.1:8080");
  msg.addAttribute("FromAddr", "127.0.0.1:8081");
  std::stringstream sstr;
  sstr << msg.bodyLength();
  msg.addAttribute("file", filename);
  msg.addAttribute("file-length", sizeString);
  msg.addAttribute("Content-Length", sstr.str());


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

//Carry out file transfer for package check out
bool Server::FileCheckOut(const std::string& filename, Socket& socket) {

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
#ifdef TEST_SERVER
int main()
{
  std::cout << "\n Server started\n ";
  
    SocketSystem ss;
    SocketListener sl(8080, Socket::IP6);
    Server server;

    std::thread t1(
      [&]() { server.execute(); });

    sl.start(server);

    BlockingQueue<HttpMessage>& sendQ = server.sendrQ();
    BlockingQueue<HttpMessage>& recvQ = server.recvrQ();

    std::string cmd;
    std::getline(std::cin, cmd);

    if (cmd == "quit") {
      HttpMessage msg;
      msg.addBody("");
      msg.addAttribute("Command", cmd);
      msg.addAttribute("ToAddr", "127.0.0.1:8080");
      msg.addAttribute("FromAddr", "127.0.0.1:8081");
      msg.addAttribute("Mode", "OneWay");
      msg.addContentLength();
      sendQ.enQ(msg);
    }
    t1.join();
 
  return 0;
}
#endif