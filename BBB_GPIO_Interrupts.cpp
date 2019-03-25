

#include "BBB_GPIO_Interrupts.hpp"

#include <stdexcept>
#include <fstream>
#include <iostream>
#include <algorithm>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>

BBB_GPIO_Interrupts::BBB_GPIO_Interrupts() {

  if ((_epfd = epoll_create(1)) < 0)
    throw std::runtime_error("Unable to create epoll");

}

void BBB_GPIO_Interrupts::addPin(BBB_GPIO_pin& pin) {

  _pins.push_back(&pin);

  struct epoll_event ev;
  ev.events = EPOLLPRI;
  ev.data.ptr = &pin;

  if(epoll_ctl(_epfd, EPOLL_CTL_ADD, pin._vfd, &ev) == -1)
    throw std::runtime_error("Error adding epoll even of "
        + pin.getPosition());
}

void BBB_GPIO_Interrupts::delPin(BBB_GPIO_pin& pin) {

  _pins.erase(std::remove(_pins.begin(), _pins.end(), &pin), _pins.end());

  if(epoll_ctl(_epfd, EPOLL_CTL_DEL, pin._vfd, NULL) == -1)
    throw std::runtime_error("Error deleting epoll even of "
        + pin.getPosition());
}

BBB_GPIO_pin* BBB_GPIO_Interrupts::edgeWait(int msTimeout) {

  // reset all the values
  for (BBB_GPIO_pin* pa : _pins)
    pa->getValue();

  struct epoll_event event;

  int ret = epoll_wait(_epfd, &event, 1, msTimeout);
  if (ret < 0)
    throw std::runtime_error("Error waiting for edge events");

  // return pin if not timeout
  return ret ? (BBB_GPIO_pin*)event.data.ptr : nullptr;

}

