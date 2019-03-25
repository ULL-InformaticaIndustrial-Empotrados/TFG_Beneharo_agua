
#include "BBB_GPIO_pin.hpp"

#include <stdexcept>
#include <fstream>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

const std::vector<BBB_GPIO_pin::pinDatum> BBB_GPIO_pin::pinsData = {
  {"P8_03", 38},
  {"P8_04", 39},
  {"P8_05", 34},
  {"P8_06", 35},
  {"P8_07", 66},
  {"P8_08", 67},
  {"P8_09", 69},
  {"P8_10", 68},
  {"P8_11", 45},
  {"P8_12", 44},
  {"P8_13", 23},
  {"P8_14", 26},
  {"P8_15", 47},
  {"P8_16", 46},
  {"P8_17", 27},
  {"P8_18", 65},
  {"P8_19", 22},
  {"P8_20", 63},
  {"P8_21", 62},
  {"P8_22", 37},
  {"P8_23", 36},
  {"P8_24", 33},
  {"P8_25", 32},
  {"P8_26", 61},
  {"P8_27", 86},
  {"P8_28", 88},
  {"P8_29", 87},
  {"P8_30", 89},
  {"P8_31", 10},
  {"P8_32", 11},
  {"P8_33", 9},
  {"P8_34", 81},
  {"P8_35", 8},
  {"P8_36", 80},
  {"P8_37", 78},
  {"P8_38", 79},
  {"P8_39", 76},
  {"P8_40", 77},
  {"P8_41", 74},
  {"P8_42", 75},
  {"P8_43", 72},
  {"P8_44", 73},
  {"P8_45", 70},
  {"P8_46", 71},
  {"P9_11", 30},
  {"P9_12", 60},
  {"P9_13", 31},
  {"P9_14", 50},
  {"P9_15", 48},
  {"P9_16", 51},
  {"P9_17", 5},
  {"P9_18", 4},
  {"P9_19", 13},
  {"P9_20", 12},
  {"P9_21", 3},
  {"P9_22", 2},
  {"P9_23", 49},
  {"P9_24", 15},
  {"P9_25", 117},
  {"P9_26", 14},
  {"P9_27", 115},
  {"P9_28", 113},
  {"P9_29", 111},
  {"P9_30", 112},
  {"P9_31", 110},
  {"P9_41", 20},
  {"P9_42", 7},
  {"P9_91", 116},
  {"P9_92", 114},
};

const std::string BBB_GPIO_pin::gpioBasePath = "/sys/class/gpio/";


BBB_GPIO_pin::BBB_GPIO_pin(std::string pinPosition) {
  // we have to iterate over the vector to see if name is valid
  std::size_t i;
  for(i = 0;
      (i < pinsData.size()) && (pinsData[i].name != pinPosition);
      i++);

  if (i == pinsData.size()) { // pin not found
    throw std::invalid_argument("Pin '" + pinPosition
        + "' is not valid");
  }
  _ind = i;

  // try to configure pinmux
  _pinmuxPath = "/sys/devices/platform/ocp/ocp:" + getPosition()
      + "_pinmux/state";

  setPullMode("gpio");

  // try to export the pin
  std::string numberStr = std::to_string(getNumber());
  _gpioPath = gpioBasePath + "gpio" + numberStr + "/";

  // first lets see if it is already exported
  std::ifstream fValue(_gpioPath + "value");
  if (!fValue.good()) { // not exported

    std::ofstream fExport(gpioBasePath + "export");
    if (!fExport.good())
      throw std::runtime_error("Not possible to acces GPIO export file");
    fExport << numberStr << std::endl;
    if (!fExport.good())
      throw std::invalid_argument("Not possible to export pin '"
          + pinPosition + "' (" + numberStr + ")");
    fExport.close();
  }
  fValue.close();

  std::string valPath = _gpioPath + "value";

  _vfd = open(valPath.c_str(), O_RDWR | O_NONBLOCK);
  if (_vfd == -1)
    throw std::runtime_error("Unable to open value file of "
        + getPosition() + ":" + valPath);

  // set as input
  setDirection("in");

}

BBB_GPIO_pin::~BBB_GPIO_pin() {
  close(_vfd);
  _vfd = -1;
}

void BBB_GPIO_pin::setDirection(std::string direction) {

  if (isDirectionIn() && direction == "out")
    setEdge("none");

  std::string dirPath = _gpioPath + "direction";

  std::ofstream fDir(dirPath);
  if (!fDir.good())
    throw std::runtime_error("Unable to open direction file for "
      + getPosition() + ":" + dirPath);

  fDir << direction << std::endl;
  if (!fDir.good())
    throw std::runtime_error("Unable to set direction '" + direction
        + "' of " + getPosition() + ":" + dirPath);
  fDir.close();

}

bool BBB_GPIO_pin::isDirectionIn() {
  std::string dirPath = _gpioPath + "direction";

  std::ifstream fDir(dirPath);
  if (!fDir.good())
    throw std::runtime_error("Unable to open direction file for "
      + getPosition() + ":" + dirPath);

  std::string direction;
  fDir >> direction;
  if (!fDir.good())
    throw std::runtime_error("Unable to read direction of "
        + getPosition() + ":" + dirPath);
  fDir.close();

  return direction == "in";

}


void BBB_GPIO_pin::setValue(int value) {

  char v = value ? '1' : '0';
  if (write(_vfd, &v, 1) != 1)
    throw std::runtime_error("Unable to set value '"
        + std::to_string(value)
        + "' of " + getPosition() + ":" + _gpioPath + "value");

}

bool BBB_GPIO_pin::getValue() {

  char value;
  lseek(_vfd, SEEK_SET, 0);

  if (read(_vfd, &value, 1) != 1)
    throw std::runtime_error("Unable to read value of "
        + getPosition() + ":" + _gpioPath + "value");

  return value == '1';
}

void BBB_GPIO_pin::setPullMode(std::string mode) {

  std::ofstream fMux(_pinmuxPath);
  if (!fMux.good())
    throw std::invalid_argument("Unable to open pinmux file of "
        + getPosition() + ": " + _pinmuxPath);
  fMux << mode << std::endl;
  if (!fMux.good())
    throw std::invalid_argument("Unable to set pinmux of "
        + getPosition() + " as '" + mode + "'");
  fMux.close();

}

std::string BBB_GPIO_pin::getPullMode() {

  std::ifstream fMux(_pinmuxPath);
  if (!fMux.good())
    throw std::invalid_argument("Unable to open pinmux file of "
        + getPosition() + ": " + _pinmuxPath);

  std::string mode;
  fMux >> mode;
  if (!fMux.good())
    throw std::invalid_argument("Unable to read pinmux of "
        + getPosition() + ": " + _pinmuxPath);
  fMux.close();

  return mode;
}

void BBB_GPIO_pin::setActiveLow(bool toLow) {
  std::string lowPath = _gpioPath + "active_low";

  std::ofstream fLow(lowPath);
  if (!fLow.good())
    throw std::runtime_error("Unable to open active_low file of "
      + getPosition() + ":" + lowPath);

  fLow << (toLow ? "1" : "0") << std::endl;
  if (!fLow.good())
    throw std::runtime_error("Unable to set active_low value '"
        + std::to_string(toLow)
        + "' of " + getPosition() + ":" + lowPath);
  fLow.close();

}

bool BBB_GPIO_pin::isActiveLow() {
  std::string lowPath = _gpioPath + "active_low";

  std::ifstream fLow(lowPath);
  if (!fLow.good())
    throw std::runtime_error("Unable to open active_low file of "
      + getPosition() + ":" + lowPath);

  int value;
  fLow >> value;
  if (!fLow.good())
    throw std::runtime_error("Unable to read active_low value of "
        + getPosition() + ":" + lowPath);
  fLow.close();

  return value;
}

void BBB_GPIO_pin::setEdge(std::string edgeMode) {

  // it must be input before set edge
  setDirection("in");

  std::string edgePath =  _gpioPath + "edge";
  std::ofstream fEdge(edgePath);
  if (!fEdge.good())
    throw std::runtime_error("Unable to open edge file of "
      + getPosition() + ":" + edgePath);
  fEdge << edgeMode << std::endl;
  if (!fEdge.good())
    throw std::runtime_error("Unable to set edge mode '"
        + edgeMode + "' of " + getPosition() + ":" + edgePath);
  fEdge.close();

}

