:: ###########################
:: ## DOWNLOAD DEPENDENCIES ##
:: ###########################

Mkdir lib 
Mkdir lib\googletest
git clone https://github.com/google/googletest.git lib/googletest

:: #################
:: ##  CONFIGURE  ##
:: #################
echo "CONFIGURING"

Mkdir build 

cmake --version

:: #############
:: ##  BUILD  ##
:: #############
echo "BUILDING"
cmake --build build --target all -- VERBOSE=1

:: ############
:: ##  TEST  ##
:: ############
echo "TESTING"
cmake --build build --target test
