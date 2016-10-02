#ifndef XMLMSGGENERATOR_H
#define XMLMSGGENERATOR_H
//////////////////////////////////////////////////////////////////////////////////////////
// XmlMsgGenerator.h - Used for generating and parsing XML body for HttpMessages        //
// ver 1.0                                                                              //
//                                                                                      //
// Application: Project 4 for CIS 687, Spring 2016                                      //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015                         //
// Author:      Deepika Lakshmanan                                                      //
//              dlakshma@syr.edu                                                        //
//////////////////////////////////////////////////////////////////////////////////////////
/*
  Module Operations:
  ==================
  This XMLResponeBodyGenerator class is an application specific class. It is used to
  make the generation and parsion og XML data easy. The XML data used is for sending information
  about packages and it's dependencies.

  Public Interface:
  =================
  string getRequestBodyForCheckIn(Package checkInPackage, vector<Package> dependencies); //Construct XML body for check-in messages
  string getRequestBodyForCheckOut(Package pkg, std::string deps);                       //Construct XML body for check-out messages

  Required Files:
  ===============
  XmlMsgGenerator.h, XmlMsgGenerator.cpp,
  XmlDocument.h, XmlDocument.cpp,
  XmlElement.h, XmlElement.cpp,
  xmlElementParts.h, xmlElementParts.cpp

  Build Command:
  ==============
  Build Command: devenv RemoteCodeRepository.sln /rebuild debug /project XmlMsgGenerator/XmlMsgGenerator.vcxproj

  Maintenance History:
  ====================
  ver 1.0 : 2 May 2016 - first release
*/

#include <iostream>
#include <map>
#include <vector>

using namespace std;

struct Package {
  string name;
  string version;
  string status;

  bool operator==(const Package &other) const
  {
    return (name == other.name
      && version == other.version
      && status == other.status);
  }
};

class XmlMsgGenerator {

public:
  XmlMsgGenerator();
  ~XmlMsgGenerator();

  string getRequestBodyForCheckIn(Package checkInPackage, vector<Package> dependencies);
  string getRequestBodyForCheckOut(Package pkg, std::string deps);
};


#endif
