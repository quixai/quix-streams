%module csTransport

%include "std_vector.i"
%include "typemaps.i"
%include "stdint.i"
%include "std_string.i"
%include "windows.i"

// Instantiate templates used by example
namespace std {
   %template(IntVector) vector<int>;
   %template(DoubleVector) vector<double>;
}
// Add necessary symbols to generated header

%include "transport/ParameterDataRaw.hpp"
%include "transport/Transport.hpp"

// %typemap(cstype) std::map<std::string, std::string>    "System.Collections.Generic.Dictionary<String, String>"
// %typemap(csout, excode=SWIGEXCODE)
// std::map<std::string, std::string>
// {
//    $csclassname ret = new System.Collections.Generic.Dictionary<String, String>();
//    for (iter = $1.begin(); iter != $1.end(); ++iter) {
//       PyDict_SetItem($result, PyUnicode_FromString(iter->first.c_str()), PyUnicode_FromString(iter->second.c_str()));
//    }

//    return ret;
// }

%{
#include <vector>
#include <map>
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

%unignore ParameterDataRaw::ParameterDataRaw();
%unignore ParameterDataRaw::~ParameterDataRaw();

%rename ("GetNumerics") ParameterDataRaw::getNumerics();
%rename ("GetMap") ParameterDataRaw::getMap();

%rename ("DoubleVal") ParameterDataRaw::doubleVal(int);



%unignore Transport;

%unignore Transport::Transport();
%unignore Transport::~Transport();

%rename ("Hello") Transport::hello(int);
%rename ("Hello") Transport::hello(int64_t);

%rename ("GetPDR") Transport::getPDR() const;

} // namespace foo

// Process symbols in header


%unignore ""; // unignore all
