#include "transport/ParameterDataRaw.hpp"

#include <iostream>

namespace transport {

ParameterDataRaw::ParameterDataRaw() {
  numeric_values.push_back(0.);
  numeric_values.push_back(1.);
  numeric_values.push_back(2.);

  map_values["val0"] = "0.";
  map_values["val1"] = "1.";
  map_values["val2"] = "2.";
}

ParameterDataRaw::~ParameterDataRaw() {}


double ParameterDataRaw::doubleVal(int index) const{
  return numeric_values[index];
}

std::vector<double> ParameterDataRaw::getNumerics(){
  return numeric_values;
}

std::map<std::string, std::string> ParameterDataRaw::getMap(){
  return map_values;
}




} // namespace foo
