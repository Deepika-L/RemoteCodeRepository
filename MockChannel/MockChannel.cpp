////////////////////////////////////////////////////////////////////////////////////
// MockChannel.cpp - Used to Transfer messages between native client code and     //
//             managed C++ code with GUI                                          //
// Application: Project 4 for CIS 687, Spring 2016                                //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015                   //
// Source:      Jim Fawcett, Syracuse University                                  //
// Author:      Deepika Lakshmanan                                                //
//              dlakshma@syr.edu                                                  //
////////////////////////////////////////////////////////////////////////////////////

#define IN_DLL
#include <string>
#include <thread>
#include <iostream>
#include "MockChannel.h"
#include "Cpp11-BlockingQueue.h"
#include "../StringClient/Client.h"
#include "../HttpMessage/HttpMessage.h"

using BQueue = BlockingQueue < HttpMessage >;


// Sendr class - accepts messages from client for consumption by MockChannel
class Sendr : public ISendr
{
public:
  void postMessage(HttpMessage& msg);
  BQueue& queue();
private:
  BQueue sendQ_;
};

void Sendr::postMessage(HttpMessage& msg)
{
  sendQ_.enQ(msg);
}

BQueue& Sendr::queue() { return sendQ_; }


// Recvr class - accepts messages from MockChanel for consumption by client
class Recvr : public IRecvr
{
public:
  HttpMessage getMessage();
  BQueue& queue();
private:
  BQueue recvQ_;
};

HttpMessage Recvr::getMessage()
{
  return recvQ_.deQ();
}

BQueue& Recvr::queue()
{
  return recvQ_;
}


// MockChannel class - reads messages from Sendr and writes messages to Recvr
class MockChannel : public IMockChannel
{
public:
  MockChannel(ISendr* pSendr, IRecvr* pRecvr);
  void start();
  void stop();
private:
  std::thread thread_;
  ISendr* pISendr_;
  IRecvr* pIRecvr_;
  Client* pClient_;
  bool stop_ = false;
};


//Pass pointers to Sender and Receiver
MockChannel::MockChannel(ISendr* pSendr, IRecvr* pRecvr) : pISendr_(pSendr), pIRecvr_(pRecvr) {

  pClient_ = new Client();
}


//Creates thread to read from sendQ and echo back to the recvQ
void MockChannel::start()
{
  std::cout << "\n  MockChannel starting up";
  thread_ = std::thread(
    [this] {
    Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);
    Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
    Client* pClient = dynamic_cast<Client*>(pClient_);

    SocketSystem ss;
    SocketListener sl(8081, Socket::IP6);

    std::thread t1(
      [&]() { pClient->execute(); });

    sl.start(*pClient);
  
    if (pSendr == nullptr || pRecvr == nullptr)
    {
      std::cout << "\n  failed to start Mock Channel\n\n";
      return;
    }
    BQueue& sendQ = pSendr->queue();
    BlockingQueue < HttpMessage >& clientsendQ = pClient->sendrQ();
    BlockingQueue < HttpMessage >& clientrecvQ = pClient->recvrQ();
    BQueue& recvQ = pRecvr->queue();

    while (!stop_)
    {
      std::cout << "\n  channel deQing message";
      HttpMessage sendmsg = sendQ.deQ();  // will block here so send quit message when stopping
      std::cout << "\n  channel enQing message" << sendmsg.buildMessage();
      clientsendQ.enQ(sendmsg);
     
      HttpMessage recvmsg = clientrecvQ.deQ();
      std::cout << "\n\nMockchannel recieved msg: " << recvmsg.buildMessage();
      recvQ.enQ(recvmsg);
      if (recvmsg.findValue("Command") == "quit") {
        break;
      }
    }
    t1.detach();
    std::cout << "\n  Server stopping\n\n";
  });
}

//Signal server thread to stop
void MockChannel::stop() { stop_ = true; }

//Factory functions
ISendr* ObjectFactory::createSendr() { return new Sendr; }

IRecvr* ObjectFactory::createRecvr() { return new Recvr; }

IMockChannel* ObjectFactory::createMockChannel(ISendr* pISendr, IRecvr* pIRecvr)
{
  return new MockChannel(pISendr, pIRecvr);
}


//----------------------------------------------Test stub----------------------------------------------
#ifdef TEST_MOCKCHANNEL

int main()
{
  ObjectFactory objFact;
  ISendr* pSendr = objFact.createSendr();
  IRecvr* pRecvr = objFact.createRecvr();
  IMockChannel* pMockChannel = objFact.createMockChannel(pSendr, pRecvr);
  pMockChannel->start();
  pSendr->postMessage("Hello World");
  pSendr->postMessage("CSE687 - Object Oriented Design");
  Message msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pSendr->postMessage("stopping");
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pMockChannel->stop();
  pSendr->postMessage("quit");
  std::cin.get();
}
#endif
