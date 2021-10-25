%module pyTransport

%include "std_vector.i"
%include "typemaps.i"
%include "stdint.i"
%include "std_string.i"
%include "std_map.i"

// Instantiate templates used by example
namespace std {
   %template(IntVector) vector<int>;
   %template(DoubleVector) vector<double>;
}
// Add necessary symbols to generated header

%typemap(out) std::map<std::string, std::string> {
  $result = PyDict_New();
  std::map<std::string, std::string>::iterator iter;

  for (iter = $1.begin(); iter != $1.end(); ++iter) {
    PyDict_SetItem($result, PyUnicode_FromString(iter->first.c_str()), PyUnicode_FromString(iter->second.c_str()));
  }
}


%include "transport/ParameterDataRaw.hpp"
%include "transport/Transport.hpp"

%{
#include <vector>
#include <transport/Transport.hpp>
#include <transport/ParameterDataRaw.hpp>

using namespace transport;
using namespace std;
%}


%ignore ""; // ignore all
%define %unignore %rename("%s") %enddef

%unignore transport;
namespace transport {

%unignore ParameterDataRaw;
%unignore ParameterDataRaw::getNumerics();
%unignore ParameterDataRaw::getMap();

%unignore ParameterDataRaw::doubleVal(int);



%unignore Transport;
%unignore Transport::hello(int);
%unignore Transport::hello(int64_t);

%unignore Transport::getPDR() const;

// %rename ("get_int") Foo::getInt() const;
// %rename ("set_int") Foo::setInt(int);

// %rename ("get_int64") Foo::getInt64() const;
// %rename ("set_int64") Foo::setInt64(int64_t);

// %rename ("__str__") Foo::operator();
} // namespace foo

// Process symbols in header


%unignore ""; // unignore all
