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
make -j 8
echo "BUILDING DONE"
