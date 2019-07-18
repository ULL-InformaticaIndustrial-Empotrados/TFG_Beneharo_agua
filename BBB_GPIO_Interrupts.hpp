#ifndef _BBB_GPIO_Interrupts_
#define _BBB_GPIO_Interrupts_

#include "BBB_GPIO_pin.hpp"

#include <vector>

/** Class to manage interrupts from one o several GPIO pins
 */
class BBB_GPIO_Interrupts {

private:

  int _epfd = -1;

  std::vector<BBB_GPIO_pin*> _pins;

public:
  /** Constructor.
   *
   * Create epoll.
   */
  BBB_GPIO_Interrupts();

  /** Add pin to monitor
   */
  void addPin(BBB_GPIO_pin& pin);

  /** Delete pin to monitor
   */
  void delPin(BBB_GPIO_pin& pin);


  /** Blocks until the configured edge is detected on pin.
   *
   * @param msTimeout timeout in miliseconds (default no timeout)
   * @return pin detected or `nullptr` if timeout
   */
  BBB_GPIO_pin* edgeWait(int msTimeout = -1); //Por defecto es -1 (infinito)

};

#endif // _BBB_GPIO_Interrupts_


