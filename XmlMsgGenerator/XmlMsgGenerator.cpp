//////////////////////////////////////////////////////////////////////////////////////////
// XmlMsgGenerator.cpp - Used for generating and parsing XML body for HttpMessages      //
// ver 1.0                                                                              //
//                                                                                      //
// Application: Project 4 for CIS 687, Spring 2016                                      //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015                         //
// Author:      Deepika Lakshmanan                                                      //
//              dlakshma@syr.edu                                                        //
//////////////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include "XmlMsgGenerator.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"
#include "../XmlDocument/XmlElementParts/xmlElementParts.h"

using namespace std;
using namespace XmlProcessing;
using sPtr = std::shared_ptr < AbstractXmlElement >;


XmlMsgGenerator::XmlMsgGenerator()
{}

XmlMsgGenerator::~XmlMsgGenerator()
{}

// Construct XML body for check - in messages
string XmlMsgGenerator::getRequestBodyForCheckIn(Package checkInPackage, vector<Package> dependencies)
{
  sPtr pRoot = makeTaggedElement("Check-In");
  XmlDocument doc(XmlProcessing::makeDocElement(pRoot));
  sPtr package = makeTaggedElement("package");
  sPtr packageName = makeTaggedElement("name");
  packageName->addChild(makeTextElement(checkInPackage.name));
  package->addChild(packageName);
  sPtr packageCppLength = makeTaggedElement("version");
  packageCppLength->addChild(makeTextElement("1"));
  package->addChild(packageCppLength);
  sPtr packageHLength = makeTaggedElement("status");
  packageHLength->addChild(makeTextElement(checkInPackage.status));
  package->addChild(packageHLength);
  pRoot->addChild(package);

  sPtr dependencies_ = makeTaggedElement("dependencies");
  for (auto dep : dependencies)
  {
    sPtr depPackage = makeTaggedElement("package");
    sPtr depPackageName = makeTaggedElement("name");
    depPackageName->addChild(makeTextElement(dep.name));
    depPackage->addChild(depPackageName);
    sPtr depPackageVersion = makeTaggedElement("version");
    depPackageVersion->addChild(makeTextElement(dep.version));
    depPackage->addChild(depPackageVersion);
    sPtr depPackageStatus = makeTaggedElement("status");
    depPackageStatus->addChild(makeTextElement(dep.status));
    depPackage->addChild(depPackageStatus);
    dependencies_->addChild(depPackage);
  }
  pRoot->addChild(dependencies_);

  return doc.toString();
}

// Construct XML body for check - out messages
string XmlMsgGenerator::getRequestBodyForCheckOut(Package pkg, std::string deps)
{
  sPtr pRoot = makeTaggedElement("Check-Out");
  XmlDocument doc(XmlProcessing::makeDocElement(pRoot));
  sPtr package = makeTaggedElement("package");
  sPtr packageName = makeTaggedElement("name");
  packageName->addChild(makeTextElement(pkg.name));
  package->addChild(packageName);
  sPtr packageCppLength = makeTaggedElement("version");
  packageCppLength->addChild(makeTextElement(pkg.version));
  package->addChild(packageCppLength);
  sPtr packageHLength = makeTaggedElement("status");
  packageHLength->addChild(makeTextElement("closed"));
  package->addChild(packageHLength);
  pRoot->addChild(package);

  sPtr dependencies_ = makeTaggedElement("dependencies");

  dependencies_->addChild(makeTextElement(deps));

  pRoot->addChild(dependencies_);

  return doc.toString();
}


//----------------------------------------------Test stub----------------------------------------------
#ifdef TEST_XMLMSGGEN

int main()
{
  XmlMsgGenerator xml;

  vector<Package> dependencies;
  Package pkg;

  std::string getRequestBodyForCheckIn = xml.getRequestBodyForCheckIn(pkg, dependencies);
  std::cout << getRequestBodyForCheckIn;

  std::string deps;
  std::cout << xml.getRequestBodyForCheckOut(pkg, deps);
}

#endif