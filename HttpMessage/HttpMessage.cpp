////////////////////////////////////////////////////////////////////////////////////
// HttpMessage.cpp - Defines an HTTP Message class for peer to peer communication //
// ver 1.0                                                                        //
//                                                                                //
// Application: Project 4 for CIS 687, Spring 2016                                //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015                   //
// Author:      Deepika Lakshmanan                                                //
//              dlakshma@syr.edu                                                  //
////////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <string>
#include <sstream>
#include "HttpMessage.h"


//Add attribute to message header
void HttpMessage::addAttribute(const Attribute& attr)
{
  attributes_.push_back(attr);
}

//Add attribute to message header
void HttpMessage::addAttribute(const Name& name, const Value& value)
{
  Attribute attribute;
  attribute.first = name;
  attribute.second = value;
  attributes_.push_back(attribute);
}

//Delete attribute
void HttpMessage::delAttribute(const Name& name)
{
  Attribute at(name, "");
  for (std::vector<Attribute>::iterator iter = attributes_.begin(); iter != attributes_.end(); ++iter)
  {
    if (iter->first == at.first)
    {
      attributes_.erase(iter);
      break;
    }
  }

}

//Display http message
void HttpMessage::display() {
  for each (Attribute var in attributes_)
  {
    std::cout << var.first << ":" << var.second << "\n";
  }
  std::cout << "\n";
  for each (byte var in body_)
  {
    std::cout << var;
  }
}

//Find value of attribute
HttpMessage::Value HttpMessage::findValue(const Name & name)
{
  for each  (Attribute attr in attributes_)
  {
    if (attr.first == name) {
      return attr.second;
    }
  }
  return "";
}

//Return message attributes
HttpMessage::Attributes& HttpMessage::attributes()
{
  return attributes_;
}

//Add message body
void HttpMessage::addBody(const Body & body)
{
  body_ = body;
}

//Add content length attribute to message
void HttpMessage::addContentLength()
{
  std::stringstream ss;

  ss << bodyLength();
  addAttribute("Content-Length", ss.str());
}

//Add message body to HttpMessage
void HttpMessage::addBody(std::string body)
{
  for (size_t i = 0; i < body.length(); i++) {
    body_.push_back(body[i]);
  }
}

//Size of message body
size_t HttpMessage::bodyLength()
{
  return body_.size();
}

//Convert Http Message to string format
HttpMessage::Message HttpMessage::buildMessage()
{
  Message msg;
  for each (Attribute att in attributes_)
  {
    msg += att.first;
    msg += ":";
    msg += att.second;
    msg += "\n";
  }
  msg += "\n";

  for each (HttpMessage::byte var in body_)
  {
    msg += var;
  }
  return msg;
}

//Get body of message
std::string HttpMessage::getBody()
{
  std::string s(body_.begin(), body_.end() - 1);
  return s;
}

//Convert string message to HttpMessage format
void HttpMessage::parseMessage(Message msg)
{
  size_t i = 0;
  while (true) {
    Attribute att;

    for (; i < msg.length(); i++) {
      if (msg[i] == ':')
        break;
      att.first += msg[i];
    }

    i++;

    for (; i < msg.length(); i++) {
      if (msg[i] == '\n')
        break;
      att.second += msg[i];
    }

    i++;
    attributes_.push_back(att);
    if (msg[i] == '\n')
      break;
  }
  i++;
  strToBody(i, msg);
}

//Convert string to message body
void HttpMessage::strToBody(size_t& i, Message& msg) {
  Value len = findValue("Content-Length");
  for (int j = 0; j <= std::stoi(len); j++, i++) {
    body_.push_back(msg[i]);
  }
}

//----------------------------------------------Test stub----------------------------------------------
#ifdef TEST_HTTPMESSAGE
int main()
{
  std::cout << " HttpMessage testing\n\n";
  std::cout << " Adding attributes and message body to http message\n\n";

  HttpMessage msg;
  msg.addAttribute("Command", "GetFiles");
  msg.addAttribute("ToAddr", "127.0.0.1:8080");
  msg.addAttribute("FromAddr", "127.0.0.1:8081");
  msg.addAttribute("Mode", "OneWay");
  msg.addBody("Hello World");
  msg.addContentLength();

  std::cout << " Created http message: \n";
  msg.display();

  std::cout << "\n\n Building message into string for transfer: \n\n";
  std::string smsg = msg.buildMessage();
  std::cout << smsg;

  std::cout << "\n\n Passing message string to parser to build http message: \n\n";
  HttpMessage recTest;
  recTest.parseMessage(smsg);
  recTest.display();
  std::cout << "\n--------------------------------\n";
}
#endif