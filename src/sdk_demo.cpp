/*
 * sdk_demo.cpp
 *
 * Created on: Jun 14, 2021 13:59
 * Description:
 *
 * You can find header "wrp_sdk/mobile_base/westonrobot/mobile_base.hpp" in
 *  folder "/opt/weston_robot/include"
 *
 * Copyright (c) 2021 Weston Robot Pte. Ltd.
 */

#include <memory>
#include <iomanip>
#include <sstream>

#include "wrp_sdk/mobile_base/westonrobot/mobile_base.hpp"

using namespace westonrobot;

std::shared_ptr<MobileBase> robot;

std::string ConvertToString(double value) {
  std::stringstream stream;
  stream << std::fixed << std::setprecision(2) << value;
  return stream.str();
}

HandshakeResultType RequestControlToken() {
  // you need to gain the control token in order to control the robot to move
  auto feedback = robot->RequestControl();
  switch (feedback.code) {
    case HANDSHAKE_RESULT_ROBOT_BASE_NOT_ALIVE:
      std::cout << "RobotBaseNotAlive" << std::endl;
      break;
    case HANDSHAKE_RESULT_CONTROL_ACQUIRED:
      std::cout << "ControlAcquired" << std::endl;
      break;
    case HANDSHAKE_RESULT_CONTROL_REJECTED_ROBOT_BASE_FAULT:
      std::cout << "ControlRejected_RobotBaseFault" << std::endl;
      break;
    case HANDSHAKE_RESULT_CONTROL_REJECTED_RC_HALT_TRIGGERED:
      std::cout << "ControlRejected_RcHaltTriggered" << std::endl;
      break;
    case HANDSHAKE_RESULT_CONTROL_REJECTED_RC_CONTROL_ACTIVE:
      std::cout << "ControlRejected_RcControlActive" << std::endl;
      break;
    case HANDSHAKE_RESULT_CONTROL_REJECTED_TOKEN_TRANSFER_INTERRUPTED:
      std::cout << "ControlRejected_TokenTransferInterrupted" << std::endl;
      break;
    case HANDSHAKE_RESULT_CONTROL_REQUEST_TIMEOUT:
      std::cout << "ControlRequestTimeout" << std::endl;
      break;
  }
  return feedback;
}

void ControlLostCallback(void) {
  std::cout << "SDK has lost control!" << std::endl;

  // you can try to regain the control token automatically or notify your
  // upper-level navigation system to respond properly
  HandshakeResultType feedback;
  do {
    std::cout << "Trying to regain control..." << std::endl;
    feedback = RequestControlToken();
    sleep(5);
  } while (feedback.code != HANDSHAKE_RESULT_CONTROL_ACQUIRED);
}

int main(int argc, char** argv) {
  /* Process arguments */
  std::string device_name;
  if (argc == 2) {
    device_name = {argv[1]};
    std::cout << "Specified CAN: " << device_name << std::endl;
  } else {
    std::cout << "Usage: sdk_demo <interface>" << std::endl
              << "Example 1: ./sdk_demo can0" << std::endl;
    return -1;
  }

  /* Create a mobile base object and try to gain control token */
  robot = std::make_shared<MobileBase>();
  robot->Connect(device_name);

  auto feedback = RequestControlToken();
  if (feedback.code != HANDSHAKE_RESULT_CONTROL_ACQUIRED) {
    std::cout << "Failed to gain control token, robot will not be controlled "
                 "by the SDK"
              << std::endl;
  }

  /* Now you can enter the main control loop*/
  ZVector3 linear, angular;
  linear.x = 0.0;
  linear.y = 0.0;
  linear.z = 0.0;

  angular.x = 0.0;
  angular.y = 0.0;
  angular.z = 0.0;

  while (true) {
    robot->SetMotionCommand(linear, angular);

    // get robot and sensor state
    SystemStateMsg system_state = robot->GetSystemState();
    MotionStateMsg motion_state = robot->GetMotionState();
    LightStateMsg light_state = robot->GetLightState();
    UltrasonicDataMsg ultrasonic_data = robot->GetUltrasonicData();
    std::vector<TofDataMsg> tof_data = robot->GetTofData();

    // print the aquired information
    std::cout << "Linear: "
              << ConvertToString(motion_state.actual_motion.linear.x) << ", ";
    std::cout << "Angular: "
              << ConvertToString(motion_state.actual_motion.angular.z) << " ; ";

    std::cout << "Ultrasonic: ";
    for (int i = 0; i < 7; ++i)
      std::cout << i << ":" << std::setw(3) << ultrasonic_data.distance[i]
                << ", ";
    std::cout << 7 << ":" << std::setw(3) << ultrasonic_data.distance[7]
              << " ; ";
    printf("TOF1: %2d, TOF2: %2d; ", tof_data[0].distance,
           tof_data[1].distance);
    std::cout << "TOF1: " << std::setw(3) << ultrasonic_data.distance[0]
              << ", ";
    std::cout << "TOF2: " << std::setw(3) << ultrasonic_data.distance[1]
              << " ; ";
    std::cout << "Battery: " << system_state.battery_state.voltage << std::endl;

    usleep(100000);
  }
}
