//////////////////////////////////////////////////////////////////////////////////////
// Repository.cpp - Repository class that handles check-in, check-out and management//
//                  of package versions                                             //
//                  ver 1.0                                                         //
//                                                                                  //
// Application: Project 4 for CIS 687, Spring 2016                                  //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015                     //
// Author:      Deepika Lakshmanan                                                  //
//              dlakshma@syr.edu                                                    //
//////////////////////////////////////////////////////////////////////////////////////

#include <unordered_map>
#include <vector>
#include <io.h>
#include "Metadata.h"
#include "Repository.h"
#include "../XmlMsgGenerator/XmlMsgGenerator.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"
#include "../XmlDocument/XmlElementParts/xmlElementParts.h"
#include "../FileSystem/FileSystem.h"
#include "../HttpMessage/HttpMessage.h"


using namespace std;
using namespace XmlProcessing;
using namespace FileSystem;
using sPtr = std::shared_ptr < AbstractXmlElement >;

//Create new instance of repository
Repository::Repository() {
  buildDependencies();
  buildVersions();
}

//Return handle to dependency map
std::unordered_map<Package, vector<Package>>& Repository::getPkgDependencies()
{
  return pkgDependencies;
}

//Return handle to versioning data
std::unordered_map<std::string, std::vector<std::string>>& Repository::getPkgVersions()
{
  return pkgVersions;
}

//Gets dependencies from xml data
vector<Package> Repository::getdependencies(XmlDocument* doc) {
  vector<Package> deps;
  vector<sPtr> pkgs = doc->element("dependencies").descendents().select();

  for (auto package : pkgs)
  {
    Package package_;
    vector<sPtr> _p = package->children();
    for (auto packageInfo : _p)
    {
      if (packageInfo->tag() == "name")
      {
        vector<sPtr> packageInfoName = packageInfo->children();
        for (auto child : packageInfoName)
          package_.name = child->value().erase(child->value().find_last_not_of(" \n\r\t") + 1);
      }
      if (packageInfo->tag() == "version")
      {
        vector<sPtr> packageInfoVersion = packageInfo->children();
        for (auto child : packageInfoVersion)
          package_.version = child->value().erase(child->value().find_last_not_of(" \n\r\t") + 1);
      }
      if (packageInfo->tag() == "status")
      {
        vector<sPtr> packageInfoStatus = packageInfo->children();
        for (auto child : packageInfoStatus)
          package_.status = child->value().erase(child->value().find_last_not_of(" \n\r\t") + 1);
      }
    }
    if (package_.name != "")
      deps.push_back(package_);
  }

  return deps;
}

//Constructs dependency data from existing repository packages when server is started up
void Repository::buildDependencies() {
  std::vector<std::string> currfiles;
  getAllFiles(Path::getFullFileSpec(Dir), &currfiles, "*.xml");

  for (auto file : currfiles) {
    vector<Package> deps;
    Package package;

    XmlDocument doc(file, XmlDocument::file);
    deps = getdependencies(&doc);

    vector<sPtr> pkginfo = doc.element("package").descendents().select();
    package.name = pkginfo[1]->value().erase(pkginfo[1]->value().find_last_not_of(" \n\r\t") + 1);
    package.version = pkginfo[3]->value().erase(pkginfo[3]->value().find_last_not_of(" \n\r\t") + 1);
    package.status = pkginfo[5]->value().erase(pkginfo[5]->value().find_last_not_of(" \n\r\t") + 1);

    std::pair<Package, vector<Package>> depentry;
    depentry.first = package;
    depentry.second = deps;

    pkgDependencies.insert(depentry);
  }
};

//Constructs versioning data from existing repository packages when server is started up
void Repository::buildVersions() {
  std::vector<std::string> currfiles;

  getAllFiles(Path::getFullFileSpec(Dir), &currfiles, "*.xml");

  for (auto file : currfiles) {
    std::string pkgname;
    std::vector<std::string> versions;

    XmlDocument doc(file, XmlDocument::file);

    vector<sPtr> pkginfo = doc.element("package").descendents().select();
    pkgname = pkginfo[1]->value().erase(pkginfo[1]->value().find_last_not_of(" \n\r\t") + 1);
    std::string version = pkginfo[3]->value().erase(pkginfo[3]->value().find_last_not_of(" \n\r\t") + 1);

    if (pkgVersions.find(pkgname) != pkgVersions.end()) {
      pkgVersions[pkgname].push_back(version);
    }
    else {
      std::pair<std::string, std::vector<std::string>> entry;
      entry.first = pkgname;
      entry.second.push_back(version);
      pkgVersions.insert(entry);
    }
  }
}

//Returns latest version of package in repository
int Repository::latestVersion(std::string pkgname) {
  std::string ver;
  int version = 1;
  if (pkgVersions.find(pkgname) != pkgVersions.end()) {
    ver = pkgVersions[pkgname].back();
    version = std::stoi(ver);
    version++;
  }
  return version;
}

//Adds version data to associated package
void Repository::addVersion(Package pkg) {

  if (pkgVersions.find(pkg.name) != pkgVersions.end()) {
    pkgVersions[pkg.name].push_back(pkg.version);
  }
  else {
    std::pair<std::string, std::vector<std::string>> entry;
    entry.first = pkg.name;
    entry.second.push_back(pkg.version);
    pkgVersions.insert(entry);
  }
}

//Adds dependency data to associated package
void Repository::addDeps(Package pkg, vector<Package> deps) {
  std::pair<Package, vector<Package>> depentry;
  depentry.first = pkg;
  depentry.second = deps;

  pkgDependencies.insert(depentry);
}

//Parse check in msg to retrieve package info
Package Repository::parseCheckInMsg(std::string requestBody)
{
  Package pkg;
  XmlDocument doc(requestBody);
  vector<sPtr> package = doc.element("package").select();
  sPtr package_ = package[0];

  for (auto packageInfo : package_->children())
  {
    if (packageInfo->tag() == "name")
    {
      vector<sPtr> packageInfoName = packageInfo->children();
      for (auto child : packageInfoName)
        pkg.name = child->value().erase(child->value().find_last_not_of(" \n\r\t") + 1);
    }
    if (packageInfo->tag() == "version")
    {
      vector<sPtr> packageInfoVersion = packageInfo->children();
      for (auto child : packageInfoVersion)
        pkg.version = child->value().erase(child->value().find_last_not_of(" \n\r\t") + 1);
    }
    if (packageInfo->tag() == "status")
    {
      vector<sPtr> packageInfoStatus = packageInfo->children();
      for (auto child : packageInfoStatus)
        pkg.status = child->value().erase(child->value().find_last_not_of(" \n\r\t") + 1);
    }
  }

  return pkg;
}

//Parse check in msg to retrieve package dependency info
vector<Package> Repository::parseCheckInMsgDeps(std::string requestBody)
{
  vector<Package> dependencyPackages;
  XmlDocument doc(requestBody);
  vector<sPtr>  dependencies = doc.element("dependencies").descendents().select();
  for (auto package : dependencies)
  {
    Package package_;
    vector<sPtr> _p = package->children();
    for (auto packageInfo : _p)
    {
      if (packageInfo->tag() == "name")
      {
        vector<sPtr> packageInfoName = packageInfo->children();
        for (auto child : packageInfoName)
          package_.name = child->value().erase(child->value().find_last_not_of(" \n\r\t") + 1);
      }
      if (packageInfo->tag() == "version")
      {
        vector<sPtr> packageInfoVersion = packageInfo->children();
        for (auto child : packageInfoVersion)
          package_.version = child->value().erase(child->value().find_last_not_of(" \n\r\t") + 1);
      }
      if (packageInfo->tag() == "status")
      {
        vector<sPtr> packageInfoStatus = packageInfo->children();
        for (auto child : packageInfoStatus)
          package_.status = child->value().erase(child->value().find_last_not_of(" \n\r\t") + 1);
      }
    }
    if (package_.name != "")
      dependencyPackages.push_back(package_);
  }

  return dependencyPackages;
}

// Recursively get all files from root path
void Repository::getAllFiles(std::string sDir, std::vector<std::string>* files, std::string fPattern)
{
  // Find the subfolders in the folder that is passed in.
  std::vector<std::string> d = Directory::getDirectories(sDir);
  size_t numDirs = d.size();

  for (size_t i = 2; i < numDirs; i++)
  {
    // Find all the files in the subfolder.
    std::vector<std::string> f = Directory::getFiles(sDir + d[i], fPattern);
    size_t numFiles = f.size();

    for (size_t j = 0; j < numFiles; j++)
    {
      files->push_back(sDir + d[i] + "\\" + f[j]);
    }
    getAllFiles(sDir + d[i], files, fPattern);
  }
}

//Check in package with dependencies contained in Http msg
void Repository::checkIn(HttpMessage msg) {
  std::string msgbody = msg.getBody();
  Package pkg = parseCheckInMsg(msgbody);
  vector<Package> deps = parseCheckInMsgDeps(msgbody);

  int version = latestVersion(pkg.name);
  pkg.version = std::to_string(version);

  std::string location = pkg.name + "_ver" + pkg.version;
  Directory::create(Dir + location);

  std::vector<std::string> files = Directory::getFiles("../TestFiles", "*.*");
  for (auto file : files) {
    File::copy("../TestFiles/" + file, "../RepositoryFiles/" + location + "/" + file);
    File::remove("../TestFiles/" + file);
  }
  if (pkg.status == "closed") {
    metadata.create(pkg, deps, location);

    addVersion(pkg);
    addDeps(pkg, deps);
  }
}

//Check out package with dependencies contained in Http msg
std::vector<std::string> Repository::checkOut(HttpMessage msg) {
  std::string msgbody = msg.getBody();
  Package pkg = parseCheckOutMsg(msgbody);
  std::string depsRequired = parseCheckOutMsgDeps(msgbody);

  std::string location = Dir + pkg.name + "_ver" + pkg.version;
  std::vector<std::string> files = Directory::getFiles(location, "*.h");
  std::vector<std::string> other = Directory::getFiles(location, "*.cpp");
  files.insert(std::end(files), std::begin(other), std::end(other));

  std::vector<std::string> downloads;

  for (auto file : files) {
    downloads.push_back(location + "/" + file);
  }
  if (depsRequired == "True") {
    std::vector<Package> deps = pkgDependencies[pkg];

    for (auto dep : deps) {
      Package pk = dep;
      std::string location = Dir + pk.name + "_ver" + pk.version;
      std::vector<std::string> files = Directory::getFiles(location, "*.h");
      std::vector<std::string> other = Directory::getFiles(location, "*.cpp");
      files.insert(std::end(files), std::begin(other), std::end(other));

      for (auto file : files) {
        downloads.push_back(location + "/" + file);
      }
    }

    //TODO:Add logic to get more than one level of dependencies
  }
  return downloads;
}

//Parse check out msg to retrieve package dependency info
std::string Repository::parseCheckOutMsgDeps(std::string requestBody) {
  XmlDocument doc(requestBody);
  vector<sPtr> deps = doc.element("dependencies").descendents().select();

  std::string depsRequired = deps[0]->value().erase(deps[0]->value().find_last_not_of(" \n\r\t") + 1);
  return depsRequired;
}

//Parse check out msg to retrieve package info
Package Repository::parseCheckOutMsg(std::string requestBody)
{
  Package pkg;
  XmlDocument doc(requestBody);
  vector<sPtr> package = doc.element("package").select();
  sPtr package_ = package[0];

  for (auto packageInfo : package_->children())
  {
    if (packageInfo->tag() == "name")
    {
      vector<sPtr> packageInfoName = packageInfo->children();
      for (auto child : packageInfoName)
        pkg.name = child->value().erase(child->value().find_last_not_of(" \n\r\t") + 1);
    }
    if (packageInfo->tag() == "version")
    {
      vector<sPtr> packageInfoVersion = packageInfo->children();
      for (auto child : packageInfoVersion)
        pkg.version = child->value().erase(child->value().find_last_not_of(" \n\r\t") + 1);
    }
    if (packageInfo->tag() == "status")
    {
      vector<sPtr> packageInfoStatus = packageInfo->children();
      for (auto child : packageInfoStatus)
        pkg.status = child->value().erase(child->value().find_last_not_of(" \n\r\t") + 1);
    }
  }
  return pkg;
}


//----------------------------------------------Test stub----------------------------------------------
#ifdef TEST_REPOSITORY
int main()
{
  cout << "\n\n-------------------Testing Repository functions ----------------------\n\n";


  Repository repo;

  std::cout << "Constructing dependency tree of existing packages";
  repo.buildDependencies();

  std::cout << "Constructing version data of existing packages";
  repo.buildVersions();


  cout << "\n\n---------Test Check-In of a package-------------\n";

  HttpMessage msg;
  msg.addBody("");
  msg.addAttribute("Command", "CheckIn");
  msg.addAttribute("ToAddr", "127.0.0.1:8080");
  msg.addAttribute("FromAddr", "127.0.0.1:8081");
  msg.addAttribute("Mode", "OneWay");
  msg.addAttribute("Package-Location", "../ClientFiles/Logger");
  msg.addContentLength();
  //repo.checkIn(msg);


  cout << "\n\n---------Test Check-Out of a package-------------\n";

  HttpMessage msg2;
  msg2.addBody("");
  msg2.addAttribute("Command", "CheckOut");
  msg2.addAttribute("ToAddr", "127.0.0.1:8080");
  msg2.addAttribute("FromAddr", "127.0.0.1:8081");
  msg2.addAttribute("Mode", "OneWay");
  msg2.addAttribute("Package-Location", "../ClientFiles/Logger");
  msg2.addAttribute("Package-name", "Package 2_ver1");
  msg2.addAttribute("Dependencies", "True");
  msg2.addContentLength();
  //repo.checkOut(msg2);

}

#endif