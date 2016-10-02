#include "../XmlMsgGenerator/XmlMsgGenerator.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"
#include "../XmlDocument/XmlElementParts/xmlElementParts.h"
#include "../FileSystem/FileSystem.h"
#include "../HttpMessage/HttpMessage.h"
#include "../Repository/Repository.h"

#include <vector>
#include <io.h>
#include "Metadata.h"

#include <unordered_map>


using namespace std;
using namespace XmlProcessing;
using namespace FileSystem;
using sPtr = std::shared_ptr < AbstractXmlElement >;


void Metadata::create(Package package, vector<Package> dependencies, std::string location)
{
  package_ = package;
  dependencies_ = dependencies;
  std::string metadata = generateMetadata();
  saveFile(metadata, location);

}

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

void Metadata::saveFile(std::string metadata, std::string location)
{
  std::string fname = Dir + location +"/"+ package_.name + "_ver" + package_.version + ".xml"; 
  FileSystem::File file(fname);
  file.open(FileSystem::File::out);

  file.putLine(metadata);
  file.putLine("\n");
  file.close();
}






 
//------------------------------------------------Test Stub----------------------------------------------

#ifdef  TEST_METADATA


string getRequestBodyForCheckIn(Package pkg, vector<Package> deps)
{
  sPtr pRoot = makeTaggedElement("Check-In");
  XmlDocument doc(XmlProcessing::makeDocElement(pRoot));
  sPtr package = makeTaggedElement("package");
  sPtr packageName = makeTaggedElement("name");
  packageName->addChild(makeTextElement(pkg.name));
  package->addChild(packageName);
  sPtr packageCppLength = makeTaggedElement("version");
  packageCppLength->addChild(makeTextElement(pkg.version));
  package->addChild(packageCppLength);
  sPtr packageHLength = makeTaggedElement("status");
  packageHLength->addChild(makeTextElement(pkg.status));
  package->addChild(packageHLength);
  pRoot->addChild(package);

  sPtr dependencies_ = makeTaggedElement("dependencies");
  for (auto dep : deps)
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

  //std::cout << msgbody << "\n\n";
  ///////////////////////////////////////////////////
 /* Metadata meta;
  meta.create(checkInPackage, packages);
  std::cout << " Metadata file created\n";*/
  ///////////////////////////////////////////////////
  //Parsing metadata file for package and dependency info
  

  Repository repo;
  //repo.buildDependencies();
  //repo.buildVersions();

  //repo.checkIn(msg);

  /*std::string file = "../RepositoryFiles/trial.xml";
  XmlDocument doc( file, XmlDocument::file);
  vector<sPtr> package = doc.element("dependencies").descendents().select();

  std::string depss = package[0]->value().erase(package[0]->value().find_last_not_of(" \n\r\t") + 1);*/
  Package test{ "Package 3", "1", "closed" };




}

#endif //  TEST_METADATA