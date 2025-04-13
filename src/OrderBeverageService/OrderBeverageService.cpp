#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <signal.h>

#include "../utils.h"
#include "OrderBeverageHandler.h"
#include "../BeveragePreferenceService/BeveragePreferenceHandler.h" // <- ADD THIS

using json = nlohmann::json;
using apache::thrift::server::TThreadedServer;
using apache::thrift::transport::TServerSocket;
using apache::thrift::transport::TFramedTransportFactory;
using apache::thrift::protocol::TBinaryProtocolFactory;

using namespace vending_machine;

// signal handler
void sigintHandler(int sig) {
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    signal(SIGINT, sigintHandler);
    init_logger();

    // Load config
    json config_json;
    if (load_config_file("config/service-config.json", &config_json) != 0) {
        exit(EXIT_FAILURE);
    }

    int my_port = config_json["order-beverage-service"]["port"];
    int weather_service_port = config_json["weather-service"]["port"];
    std::string weather_service_addr = config_json["weather-service"]["addr"];

    int beverage_pref_port = config_json["beverage-preference-service"]["port"];
    std::string beverage_pref_addr = config_json["beverage-preference-service"]["addr"];

    // Client pools
    ClientPool<ThriftClient<WeatherServiceClient>> weather_client_pool(
        "weather-service", weather_service_addr, weather_service_port, 0, 128, 1000);

    ClientPool<ThriftClient<BeveragePreferenceServiceClient>> beverage_pref_client_pool(
        "beverage-preference-service", beverage_pref_addr, beverage_pref_port, 0, 128, 1000);

    // Start server
    TThreadedServer server(
        std::make_shared<OrderBeverageServiceProcessor>(
            std::make_shared<OrderBeverageServiceHandler>(
                &weather_client_pool,
                &beverage_pref_client_pool
            )
        ),
        std::make_shared<TServerSocket>("0.0.0.0", my_port),
        std::make_shared<TFramedTransportFactory>(),
        std::make_shared<TBinaryProtocolFactory>()
    );

    std::cout << "Starting the order-beverage server ..." << std::endl;
    server.serve();
    return 0;
}
