#ifndef MOCKCHANNEL_H
#define MOCKCHANNEL_H
////////////////////////////////////////////////////////////////////////////////////
// MockChannel.h - Used to Transfer messages between native client code and       //
//             managed C++ code with GUI                                          //
// Application: Project 4 for CIS 687, Spring 2016                                //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015                   //
// Source:      Jim Fawcett, Syracuse University                                  //
// Author:      Deepika Lakshmanan                                                //
//              dlakshma@syr.edu                                                  //
////////////////////////////////////////////////////////////////////////////////////
/*
* Module Operations :
* ==================
*  - build as DLL to show how C++\CLI client can use native code channel
*  - Channel reads from sendQ and writes to recvQ            
*
* Required Files :
* ================
*	HttpMessage.h, HttpMessage.cpp
*
* Public Interface :
* =================
* ISender
*	- postMessage - post message from the GUI to the channel queue
*
* IReceiver
*	- getMessage - get the message posted by the client as a response
*
* Build Command :
* ==============
* Build Command : devenv RemoteCodeRepository.sln /rebuild debug/ project MockChannel/MockChannel.vcxproj
*
* Maintenance History :
*====================
* ver 1.0 : 2 May 2016 -first release
*/
#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include <string>
#include "../HttpMessage/HttpMessage.h"


struct ISendr
{
  virtual void postMessage( HttpMessage&) = 0;
};

struct IRecvr
{
  virtual HttpMessage getMessage() = 0;
};

struct IMockChannel
{
public:
  virtual void start() = 0;
  virtual void stop() = 0;
};

extern "C" {
  struct ObjectFactory
  {
    DLL_DECL ISendr* createSendr();
    DLL_DECL IRecvr* createRecvr();
    DLL_DECL IMockChannel* createMockChannel(ISendr* pISendr, IRecvr* pIRecvr);
  };
}

#endif


