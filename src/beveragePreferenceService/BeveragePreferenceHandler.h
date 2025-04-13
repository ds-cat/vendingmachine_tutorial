#ifndef VENDING_MACHINE_MICROSERVICES_BEVERAGEPREFERENCEHANDLER_H
#define VENDING_MACHINE_MICROSERVICES_BEVERAGEPREFERENCEHANDLER_H

#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "../../gen-cpp/BeveragePreferenceService.h"

namespace vending_machine {

class BeveragePreferenceServiceHandler : public BeveragePreferenceServiceIf {
 public:
  BeveragePreferenceServiceHandler() {
    std::srand(std::time(nullptr)); // Seed random number generator
  }

  std::string GetBeverage(BeverageType::type btype) override {
    std::vector<std::string> hot = {"cappuccino", "latte", "espresso"};
    std::vector<std::string> cold = {"lemonade", "ice tea", "soda"};
    
    const auto& options = (btype == BeverageType::type::HOT) ? hot : cold;
    int index = std::rand() % options.size();
    return options[index];
  }
};

} // namespace vending_machine

#endif // VENDING_MACHINE_MICROSERVICES_BEVERAGEPREFERENCEHANDLER_H
