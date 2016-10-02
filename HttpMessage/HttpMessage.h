#ifndef HTTPMESSAGE_H
#define HTTPMESSAGE_H
////////////////////////////////////////////////////////////////////////////////////
// HttpMessage.h - Defines an HTTP Message class for peer to peer communication   //
// ver 1.0                                                                        //
//                                                                                //
// Application: Project 4 for CIS 687, Spring 2016                                //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015                   //
// Author:      Deepika Lakshmanan                                                //
//              dlakshma@syr.edu                                                  //
////////////////////////////////////////////////////////////////////////////////////
/*
 Module Operations:
 ==================
 The HttpMessage class provides a way of sending request/response over a network
 It comprises of a set of attributes which are key, value pairs. These attributes
 provide important information about the message being transmitted.
 It also provides a body for sending data, which can be a string or structured XML data.

 Public Interface:
 =================
 HttpMessage httpMessage;                                 //create a new HttpMessage
 void addAttribute(const Attribute& attr);                //Add attribute to message header
 void addAttribute(const Name& name, const Value& value); //Add attribute to message header
 void delAttribute(const Name & name);                    //Delete attribute
 Value findValue(const Name& name);                       //Find value of attribute
 Attributes& attributes();                                //Return message attributes
 void addBody(const Body& body);                          //Add message body
 void addContentLength();                                 //Add content length attribute to message
 void HttpMessage::addBody(std::string body);             //Add message body to HttpMessage
 size_t bodyLength();                                     //Size of message body
 Message buildMessage();                                  //Convert Http Message to string format
 std::string getBody();                                   //Get body of message
 void parseMessage(Message msg);                          //Convert string message to HttpMessage format
 void strToBody(size_t& i, Message& msg)                  //Convert string to message body
 void display();                                          //Display http message

 Required Files:
 ===============
 HttpMessage.h, HttpMessage.cpp

 Build Command:
 ==============
 Build Command: devenv RemoteCodeRepository.sln /rebuild debug /project HttpMessage/HttpMessage.vcxproj

 Maintenance History:
 ====================
 ver 1.0 : 2 May 2016 - first release

*/

#include <vector>
#include <string>
#include <iostream>


class HttpMessage
{
public:
  using byte = char;
  using Name = std::string;
  using Value = std::string;
  using Attribute = std::pair<Name, Value>;
  using Attributes = std::vector<Attribute>;
  using Body = std::vector<byte>;
  using Message = std::string;

  //message attributes

  void addAttribute(const Attribute& attr);
  void addAttribute(const Name& name, const Value& value);
  void delAttribute(const Name & name);
  Value findValue(const Name& name);
  Attributes& attributes();
  void addBody(const Body& body);
  void addContentLength();
  void HttpMessage::addBody(std::string body);
  size_t bodyLength();

  //Construct message
  Message buildMessage();
  std::string getBody();
  void parseMessage(Message msg);
  void strToBody(size_t & i, Message & msg);
  void display();

private:
  Attributes attributes_;
  Body body_;
};

#endif