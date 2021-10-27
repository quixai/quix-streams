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
cmake -DGTEST_CREATE_SHARED_LIBRARY=1 -DGTEST_LINKED_AS_SHARED_LIBRARY=1 ..

echo "CONFIGURING DONE"

:: #############
:: ##  BUILD  ##
:: #############
echo "BUILDING"
cmake --build .
echo "BUILDING DONE"
