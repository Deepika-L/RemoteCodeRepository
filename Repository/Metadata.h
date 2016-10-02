#ifndef METADATA_H
#define METADATA_H
////////////////////////////////////////////////////////////////////////////////////
// Metadata.h - Metadata class that managed metadata for repository packages      //
//              ver 1.0                                                           //
//                                                                                //
// Application: Project 4 for CIS 687, Spring 2016                                //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015                   //
// Author:      Deepika Lakshmanan                                                //
//              dlakshma@syr.edu                                                  //
////////////////////////////////////////////////////////////////////////////////////
/*
  Module Operations:
  ==================
  The Metadata class is a helper class for the Repository. It handles generation and
  creation of metadata files for checked-in packages.

  Public Interface:
  =================

  void create(Package package, vector<Package> dependencies, std::string location); //Start function that handles creation of metadata
  std::string generateMetadata();                                                   //Generates metadata for check in package and dependencies
  void saveFile(std::string metadata, std::string location);                        //Saves metadata file along with check-in files

  Required Files:
  ===============

  XmlMsgGenerator.h, XmlMsgGenerator.cpp
  XmlDocument.h, XmlDocument.cpp
  XmlElement.h, XmlElement.cpp
  XmlElementParts.h, XmlElementParts.cpp
  HttpMessage.h, HttpMessage.cpp
  FileSystem.h, FileSystem.cpp


  Build Command:
  ==============
  Build Command: devenv RemoteCodeRepository.sln /rebuild debug /project Repository/Repository.vcxproj

  Maintenance History:
  ====================
  ver 1.0 : 2 May 2016 - first release

*/
#include "../XmlMsgGenerator/XmlMsgGenerator.h"
#include <string>

class Metadata
{
  Package package_;
  vector<Package> dependencies_;
  std::string Dir = "../RepositoryFiles/";
public:

  void create(Package package, vector<Package> dependencies, std::string location);

  std::string generateMetadata();

  void saveFile(std::string metadata, std::string location);

};
#endif
