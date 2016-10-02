Project #4: Remote Code Repository

Repository Functions for Client Requests:
=========================================

-GetPackages:
Server returns a list of all closed packages from the repository. User can then choose which of these are
dependencies for any package the user wishes to check in

-CheckIn:
The CheckIn message contains package name and location obtained from the user through the GUI. The user also
specifies if he wishes to keep the check-in open or closed. If he chooses a closed check-in, all files in the location specified by user get uploaded to the repository and the package is closed from further changes. Metadata file is generated for this package.
If the user wishes to keep the check in open, the package is uploaded to the repository, but it remains open to manipulation. The user must check in the package again, and specify as closed, if he wishes to close an open check-in.

-CheckOut
This allows a user to choose any package on the repository and download to the ClientDownloads folder. The user must specify if he wishes to check out the package with or without its dependencies. If he chooses without dependencies, only the package contents are downloaded. If he chooses with dependencies, the package along with the first level of its dependencies will be downloaded.

-quit
To exit the client

-quitserver
To shutdown the server

Directory Structure
==============================
(Note: All paths are relative to the Debug folder where all exe files reside)
Server:
 
1) Repository files
   ../RepositoryFiles

Each package is stored in its own directory. The directory name is unique to each new check-in. The directory name is a combination of the package name and the version number. 
Ex: ../RepositoryFiles/<PackageName>_ver<VersionNumber>
All files associated with this package reside within the directory

2) Socket Uploads - This is where all the file uploads are done initially and then the repository 
                    moves it to the repository directories
        ../TestFiles/

3) Client Downloads
  Client downloads are stored in the following location
  ../ClientDownloads