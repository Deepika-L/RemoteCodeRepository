#ifndef SERVER_H
#define SERVER_H
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Server.h - Server class that interacts with the client and carries out Repository functions       //
// ver 1.0                                                                                           //
//                                                                                                   //
// Application: Project 4 for CIS 687, Spring 2016                                                   //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015                                      //
// Source:      Jim Fawcett, Syracuse University                                                     //
// Author:      Deepika Lakshmanan                                                                   //
//              dlakshma@syr.edu                                                                     //
///////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  Module Operations:
  ==================
  The Server class contains methods to recieve and send http-style messages to interact
  with the client and the gui with the use of blocking queues. The server implements repository
  functionality to check-in and check-out packages. It also uses the repository class to maintain
  package version data. Clients interact with the server to check in and check out packages.

  Public Interface:
  =================
  BlockingQueue<HttpMessage>& sendrQ();                              //Return handle to sender queue
  BlockingQueue<HttpMessage>& recvrQ();                              //Return handle to receiver queue
  void execute();                                                    //Start sender of server
  void operator()(Socket& socket);                                   //Start clienthandler to listen on port
  void CheckInDone();                                                //Function to execute to indicate package check-in has completed
  void FileCheckIn(HttpMessage & recvd, Socket& socket);             //Handles files for package check in
  void GetPkgs();                                                    //Returns list of packages currently in repository
  void GetPkgsChkOut();                                              //Returns list of packages available for check-out
  void sendMessage(std::string& msg, Socket & socket);               //Send message through socket
  bool sendFile(const std::string & filename, Socket & socket);      //Carry out file transfer for package check in
  bool FileCheckOut(const std::string & filename, Socket & socket);  //Carry out file transfer for package check out

  Required Files:
  ===============
  HttpMessage.h, HttpMessage.cpp,
  Repository.h, Repository.cpp,
  Metadata.h, Metadata.cpp,
  Sockets.h, Sockets.cpp
  Logger.h, Logger.cpp, Cpp11-BlockingQueue.h
  Utilities.h, Utilities.cpp

  Build Command:
  ==============
  Build Command: devenv RemoteCodeRepository.sln /rebuild debug /project StringServer/StringServer.vcxproj

  Maintenance History:
  ====================
  ver 1.0 : 2 May 2016 - first release

*/
#include "../Sockets/Sockets.h"
#include "../HttpMessage/HttpMessage.h"
#include "../Logger/Cpp11-BlockingQueue.h"
#include "../Repository/Repository.h"


class Server
{
  BlockingQueue<HttpMessage> sendQ;
  BlockingQueue<HttpMessage> recvQ;
  Repository repo;

public:
  BlockingQueue<HttpMessage>& sendrQ() { return sendQ; }
  BlockingQueue<HttpMessage>& recvrQ() { return recvQ; }
  void execute();
  void operator()(Socket& socket);
  void CheckInDone();
  void FileCheckIn(HttpMessage & recvd, Socket& socket);
  void GetPkgs();
  void GetPkgsChkOut();
  void sendMessage(std::string& msg, Socket & socket);
  bool sendFile(const std::string & filename, Socket & socket);
  bool FileCheckOut(const std::string & filename, Socket & socket);

};

#endif
