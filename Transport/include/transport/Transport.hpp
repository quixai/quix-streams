#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "transport/ParameterDataRaw.hpp"


namespace transport {
/*!
* Transport class
*/

class Transport {
 public:
  Transport();
  ~Transport();

  static void hello(int level);

  static void hello(int64_t level);

  int getInt() const;
  void setInt(int input);

  /*!
  * Get ParameterDataRaw class.
  */
  ParameterDataRaw getPDR() const;

  int64_t getInt64() const;
  void setInt64(int64_t input);

  std::string operator()() const;

 private:
  int     _intValue   = 0;
  int64_t _int64Value = 0;
};
} // namespace foo
