#ifndef CLIENT_H
#define CLIENT_H
////////////////////////////////////////////////////////////////////////////////////
// Client.h - Client class that interacts with the server and GUI                 //
// ver 1.0                                                                        //
//                                                                                //
// Application: Project 4 for CIS 687, Spring 2016                                //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015                   //
// Source:      Jim Fawcett, Syracuse University                                  //
// Author:      Deepika Lakshmanan                                                //
//              dlakshma@syr.edu                                                  //
////////////////////////////////////////////////////////////////////////////////////
/*
  Module Operations:
  ==================
  The Client class contains methods to recieve and send http-style messages to interact
  with the server and the gui with the use of blocking queues.

  Public Interface:
  =================
  BlockingQueue<HttpMessage>& sendrQ();                              //Return handle to sender queue
  BlockingQueue<HttpMessage>& recvrQ();                              //Return handle to receiver queue
  void execute();                                                    //Start sender of client
  void CheckOut(HttpMessage & send, Socket & si);                    //Build message for package check out
  void CheckIn(HttpMessage & send, Socket & si);                     //Build message for package check in
  void operator()(Socket& socket);                                   //Start clienthandler to listen on port
  void sendMessage(std::string& msg, Socket & socket);               //Send message through socket
  bool FileCheckIn(const std::string & filename, Socket & socket);   //Carry out file transfer for package check in
  void FileCheckOut(const std::string & filename, const std::string & filelength, Socket & socket);  //Carry out file transfer for package check out

  Required Files:
  ===============
  HttpMessage.h, HttpMessage.cpp
  Sockets.h, Sockets.cpp
  Logger.h, Logger.cpp, Cpp11-BlockingQueue.h
  Utilities.h, Utilities.cpp

  Build Command:
  ==============
  Build Command: devenv RemoteCodeRepository.sln /rebuild debug /project StringClient/StringClient.vcxproj

  Maintenance History:
  ====================
  ver 1.0 : 2 May 2016 - first release
*/

#include "../Sockets/Sockets.h"
#include "../HttpMessage/HttpMessage.h"
#include "../Logger/Cpp11-BlockingQueue.h"


class Client
{
  BlockingQueue<HttpMessage> sendQ;
  BlockingQueue<HttpMessage> recvQ;

public:
  BlockingQueue<HttpMessage>& sendrQ() { return sendQ; }
  BlockingQueue<HttpMessage>& recvrQ() { return recvQ; }

  void execute();
  void CheckOut(HttpMessage & send, Socket & si);
  void CheckIn(HttpMessage & send, Socket & si);
  void operator()(Socket& socket);
  void FileCheckOut(const std::string & filename, const std::string & filelength, Socket & socket);
  void sendMessage(std::string& msg, Socket & socket);
  bool FileCheckIn(const std::string & filename, Socket & socket);
};

#endif