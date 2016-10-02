#ifndef REPOSITORY_H
#define REPOSITORY_H
////////////////////////////////////////////////////////////////////////////////////
// Repository.h - Repository class that handles check-in, check-out and management//
//                of package versions                                             //
//                ver 1.0                                                         //
//                                                                                //
// Application: Project 4 for CIS 687, Spring 2016                                //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015                   //
// Author:      Deepika Lakshmanan                                                //
//              dlakshma@syr.edu                                                  //
////////////////////////////////////////////////////////////////////////////////////
/*
  Module Operations:
  ==================
  The Repository class handles the server-side operations of the remote code repository.
  Its functions include check-in and check-out of packages, versioning of packages and
  also maintaining and updating open check-ins.

  Public Interface:
  =================

  Repository();                                                                 //Create new instance of repository
  std::unordered_map<Package, vector<Package>>& getPkgDependencies();           //Return handle to dependency map
  std::unordered_map<std::string, std::vector<std::string>>& getPkgVersions();  //Return handle to versioning data
  vector<Package> getdependencies(XmlDocument* doc);                            //Gets dependencies from xml data
  void buildDependencies();                                                     //Constructs dependency data from existing repository
                                                                                  packages when server is started up
  void buildVersions();                                                         //Constructs versioning data from existing repository
                                                                                  packages when server is started up
  int latestVersion(std::string pkgname);                                       //Returns latest version of package in repository
  void addVersion(Package pkg);                                                 //Adds version data to associated package
  void addDeps(Package pkg, vector<Package> deps);                              //Adds dependency data to associated package
  Package parseCheckInMsg(std::string requestBody);                             //Parse check in msg to retrieve package info
  vector<Package> parseCheckInMsgDeps(std::string requestBody);                 //Parse check in msg to retrieve package dependency info
  void getAllFiles(std::string sDir, std::vector<std::string>* files, std::string fPattern);  // Recursively get all files from root path
  void checkIn(HttpMessage msg);                                                //Check in package with dependencies contained in Http msg
  std::vector<std::string> checkOut(HttpMessage msg);                           //Check out package with dependencies contained in Http msg
  std::string parseCheckOutMsgDeps(std::string requestBody);                    //Parse check out msg to retrieve package dependency info
  Package parseCheckOutMsg(std::string requestBody);                            //Parse check out msg to retrieve package info

  Required Files:
  ===============
  Repository.cpp
  Metadata.h, Metadata.cpp
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

#include <vector>
#include <io.h>
#include <unordered_map>
#include "../XmlMsgGenerator/XmlMsgGenerator.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"
#include "../XmlDocument/XmlElementParts/xmlElementParts.h"
#include "../FileSystem/FileSystem.h"
#include "../HttpMessage/HttpMessage.h"
#include "Metadata.h"


using namespace std;
using namespace XmlProcessing;
using namespace FileSystem;
using sPtr = std::shared_ptr < AbstractXmlElement >;


//Hash function to use Package struct with unordered_map
template <>
struct hash<Package>
{
  std::size_t operator()(const Package& k) const
  {
    using std::size_t;
    using std::hash;
    using std::string;

    // Compute individual hash values for first,
    // second and third and combine them using XOR
    // and bit shifting:

    return ((hash<string>()(k.name)
      ^ (hash<string>()(k.version) << 1)) >> 1)
      ^ (hash<string>()(k.status) << 1);
  }
};


class Repository {
  Metadata metadata;
  std::unordered_map<Package, vector<Package>> pkgDependencies;
  std::unordered_map<std::string, std::vector<std::string>> pkgVersions;
  std::string Dir = "../RepositoryFiles/";

public:

  Repository();

  std::unordered_map<Package, vector<Package>>& getPkgDependencies();

  std::unordered_map<std::string, std::vector<std::string>>& getPkgVersions();

  vector<Package> getdependencies(XmlDocument* doc);

  void buildDependencies();

  void buildVersions();

  int latestVersion(std::string pkgname);

  void addVersion(Package pkg);

  void addDeps(Package pkg, vector<Package> deps);

  Package parseCheckInMsg(std::string requestBody);

  vector<Package> parseCheckInMsgDeps(std::string requestBody);

  void getAllFiles(std::string sDir, std::vector<std::string>* files, std::string fPattern);

  void checkIn(HttpMessage msg);

  std::vector<std::string> checkOut(HttpMessage msg);

  std::string parseCheckOutMsgDeps(std::string requestBody);

  Package parseCheckOutMsg(std::string requestBody);
};
#endif
