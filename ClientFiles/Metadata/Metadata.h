#ifndef METADATA_H
#define METADATA_H

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
