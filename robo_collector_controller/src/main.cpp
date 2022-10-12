//System headers
#include <cstdint>

//Other libraries headers
#include "ros2_game_engine/communicator/Ros2Communicator.h"
#include "game_engine/Application.h"
#include "utils/ErrorCode.h"
#include "utils/Log.h"

//Own components headers
#include "robo_collector_controller/helpers/RoboCollectorControllerBuilder.h"
#include "robo_collector_controller/config/RoboCollectorControllerConfigGenerator.h"

int32_t main(int32_t argc, char **args) {
  Application app;

  const auto dependencies =
      RoboCollectorControllerConfigGenerator::generateDependencies(argc, args);
  if (ErrorCode::SUCCESS != app.loadDependencies(dependencies)) {
    LOGERR("app.loadDependencies() failed");
    return EXIT_FAILURE;
  }

  auto communicator = std::make_unique<Ros2Communicator>();
  auto game = RoboCollectorControllerBuilder::createRoboCollectorController(
      communicator);
  app.obtain(std::move(game), std::move(communicator));

  const auto cfg = RoboCollectorControllerConfigGenerator::generateConfig();
  if (ErrorCode::SUCCESS != app.init(cfg)) {
    LOGERR("app.init() failed");
    return EXIT_FAILURE;
  }

  if (ErrorCode::SUCCESS != app.run()) {
    LOGERR("app.run() failed");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
