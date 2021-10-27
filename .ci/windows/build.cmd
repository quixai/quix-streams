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

cmake --version

Mkdir build 
cd build
cmake ..

echo "CONFIGURING DONE"

:: #############
:: ##  BUILD  ##
:: #############
echo "BUILDING"
cmake --build . --config Release

echo "DIR ."
dir .
echo "DIR build"
dir bin

echo "BUILDING DONE"
