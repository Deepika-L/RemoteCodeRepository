////////////////////////////////////////////////////////////////////////////////////
// Metadata.cpp - Metadata class that managed metadata for repository packages    //
//                ver 1.0                                                         //
//                                                                                //
// Application: Project 4 for CIS 687, Spring 2016                                //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015                   //
// Author:      Deepika Lakshmanan                                                //
//              dlakshma@syr.edu                                                  //
////////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <io.h>
#include <unordered_map>
#include "Metadata.h"
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


//Start function that handles creation of metadata
void Metadata::create(Package package, vector<Package> dependencies, std::string location)
{
  package_ = package;
  dependencies_ = dependencies;
  std::string metadata = generateMetadata();
  saveFile(metadata, location);

}

//Generates metadata for check in package and dependencies
std::string Metadata::generateMetadata()
{
  sPtr pRoot = makeTaggedElement("metadata");
  XmlDocument doc(XmlProcessing::makeDocElement(pRoot));
  sPtr package = makeTaggedElement("package");

  sPtr name = makeTaggedElement("name");
  name->addChild(makeTextElement(package_.name));
  package->addChild(name);

  sPtr version = makeTaggedElement("version");
  version->addChild(makeTextElement(package_.version));
  package->addChild(version);

  sPtr status = makeTaggedElement("status");
  status->addChild(makeTextElement(package_.status));
  package->addChild(status);

  pRoot->addChild(package);

  sPtr dep = makeTaggedElement("dependencies");
  for (auto package : dependencies_)
  {
    sPtr pack = makeTaggedElement("package");
    sPtr packName = makeTaggedElement("name");
    packName->addChild(makeTextElement(package.name));
    pack->addChild(packName);
    sPtr packVersion = makeTaggedElement("version");
    packVersion->addChild(makeTextElement(package.version));
    pack->addChild(packVersion);
    sPtr packStatus = makeTaggedElement("status");
    packStatus->addChild(makeTextElement(package.status));
    pack->addChild(packStatus);
    dep->addChild(pack);
  }
  pRoot->addChild(dep);

  std::string metadata = doc.toString();
  return metadata;
}

//Saves metadata file along with check-in files
void Metadata::saveFile(std::string metadata, std::string location)
{
  std::string fname = Dir + location + "/" + package_.name + "_ver" + package_.version + ".xml";
  FileSystem::File file(fname);
  file.open(FileSystem::File::out);

  file.putLine(metadata);
  file.putLine("\n");
  file.close();
}



//------------------------------------------------Test Stub----------------------------------------------

#ifdef  TEST_METADATA


//string getRequestBodyForCheckIn(Package pkg, vector<Package> deps)
//{
//  sPtr pRoot = makeTaggedElement("Check-In");
//  XmlDocument doc(XmlProcessing::makeDocElement(pRoot));
//  sPtr package = makeTaggedElement("package");
//  sPtr packageName = makeTaggedElement("name");
//  packageName->addChild(makeTextElement(pkg.name));
//  package->addChild(packageName);
//  sPtr packageCppLength = makeTaggedElement("version");
//  packageCppLength->addChild(makeTextElement(pkg.version));
//  package->addChild(packageCppLength);
//  sPtr packageHLength = makeTaggedElement("status");
//  packageHLength->addChild(makeTextElement(pkg.status));
//  package->addChild(packageHLength);
//  pRoot->addChild(package);
//
//  sPtr dependencies_ = makeTaggedElement("dependencies");
//  for (auto dep : deps)
//  {
//    sPtr depPackage = makeTaggedElement("package");
//    sPtr depPackageName = makeTaggedElement("name");
//    depPackageName->addChild(makeTextElement(dep.name));
//    depPackage->addChild(depPackageName);
//    sPtr depPackageVersion = makeTaggedElement("version");
//    depPackageVersion->addChild(makeTextElement(dep.version));
//    depPackage->addChild(depPackageVersion);
//    sPtr depPackageStatus = makeTaggedElement("status");
//    depPackageStatus->addChild(makeTextElement(dep.status));
//    depPackage->addChild(depPackageStatus);
//    dependencies_->addChild(depPackage);
//  }
//  pRoot->addChild(dependencies_);
//
//  return doc.toString();
//}


int main() {

  vector<Package> packages;
  Package package1; package1.name = "Package1"; package1.version = "1"; package1.status = "open";
  packages.push_back(package1);
  Package package2; package2.name = "Package2"; package2.version = "2"; package2.status = "open";
  packages.push_back(package2);
  Package package3; package3.name = "Package3"; package3.version = "2"; package3.status = "closed";
  packages.push_back(package3);

  Package checkInPackage;
  checkInPackage.name = "Package 1"; checkInPackage.version = "1"; checkInPackage.status = "closed";


  HttpMessage msg;
  std::string msgbody = getRequestBodyForCheckIn(checkInPackage, packages);
  msg.addBody(msgbody);

}

#endif