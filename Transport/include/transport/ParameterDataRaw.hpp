#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

//! @namespace foo The Foo namespace
namespace transport {
//! @brief Class ParameterDataRaw.
class ParameterDataRaw {
 public:
  //! @defgroup StaticMembers Static members
  //! @{

  //! @brief Static method of Foo class.
  //! @param[in] level Scope level.
  ParameterDataRaw();

  //! @brief Static method of Foo class.
  //! @param[in] level Scope level.
  ~ParameterDataRaw();

  //! @brief Static method of Foo class.
  //! @param[in] level Scope level.
  double doubleVal(int index) const;

  /**
  *  Get numerics. Return vector of variables
  */
   std::vector<double> getNumerics();  
  /**
  * Get Map. Returns map
  */
  std::map<std::string, std::string> getMap();

  int value; 

 private:
  std::vector<double> numeric_values; 
  std::map<std::string, std::string> map_values; 
};
} // namespace foo
