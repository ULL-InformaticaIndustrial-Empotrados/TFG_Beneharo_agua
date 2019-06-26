#ifndef _BBB_GPIO_pin_
#define _BBB_GPIO_pin_

#include <cstdint>
#include <string>
#include <vector>


class BBB_GPIO_Interrupts;

/** Class that represents a General propouse Input/Output (GPIO) pin.
 *
 */
class BBB_GPIO_pin {

private:

  //friend -- access all private and protected members of the class
  friend class BBB_GPIO_Interrupts; 

  struct pinDatum {
    std::string name;
    uint8_t number;
  };
  static const std::vector<pinDatum> pinsData;
  static const std::string gpioBasePath;

  std::size_t _ind; // pin index in pinsData
  std::string _pinmuxPath; // pinmux path for this pin
  std::string _gpioPath; // GPIO directory path for this pin

  int _vfd = -1; // file descriptor of value file

public:

  /** Constructor.
   *
   * Try to exports the pin and to configure the pin-mux.
   * On any error a exception is thrown.
   *
   * @param pinPosition  Ponsition in BBB conectors in de form "P8_07"
   */
  BBB_GPIO_pin(std::string pinPosition);

  /** Destructor.
   *
   * Release filedescriptor.
   */
  ~BBB_GPIO_pin();

  /** @return pin position like "P8_09" or "P9_41" */
  std::string getPosition() { return pinsData[_ind].name; }

  /** @return kernel pin number */
  int getNumber() { return pinsData[_ind].number; }


  /** Try to set pin direction.
   *
   * On any error a exception is thrown.
   *
   * @param direction must be "in" or "out"
   */
  void setDirection(std::string direction);

  /** @return if pin is configure as input */
  bool isDirectionIn();

  /** Try to sets pin value
   *
   * On any error a exception is thrown,
   * for example if invoque on an input pin
   *
   * @param value should be 0 or 1
   */
  void setValue(int value);

  /** @return actual value on the pin */
  bool getValue();

  /** Try to set pull mode.
   *
   * @param mode "gpio_pu" form pull-up, "gpio_pd" for pull-down,
   *   "gpio" for no pull.
   */
  void setPullMode(std::string mode);

  /** @return actual pull mode in pin: "gpio_pu", "gpio_pd", "gpio" */
  std::string getPullMode();

  /** Try to set inverted logic on the pin.
   *
   * @param toLow true if you want inverted logic
   */
  void setActiveLow(bool toLow);

  /** @return if pin is in inverted logic */
  bool isActiveLow();

  /** Try to set the edge detection mode of interruption.
   *
   * Pin is configured as input.
   *
   * @param edgeMode admited values "none", "rising", "falling", "both"
   */
  void setEdge(std::string edgeMode);

};

#endif // ifndef _BBB_GPIO_pin_
