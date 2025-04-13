#include <thrift/Thrift.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PosixThreadFactory.h>

#include "../../gen-cpp/BeveragePreferenceService.h"
#include "../logger.h"
#include "BeveragePreferenceHandler.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;

int main(int argc, char** argv) {
  const int port = 9095;

  ::std::shared_ptr<vending_machine::BeveragePreferenceServiceHandler> handler(new vending_machine::BeveragePreferenceServiceHandler());
  ::std::shared_ptr<TProcessor> processor(new vending_machine::BeveragePreferenceServiceProcessor(handler));
  ::std::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  ::std::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  ::std::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TThreadPoolServer server(processor, serverTransport, transportFactory, protocolFactory);

  LOG_INFO("Starting the BeveragePreferenceService server on port {}", port);
  server.serve();
  LOG_INFO("BeveragePreferenceService server stopped.");
  return 0;
}