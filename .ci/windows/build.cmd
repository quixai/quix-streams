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
cmake -DUSE_MSVC_RUNTIME_LIBRARY_DLL=1 ..

echo "CONFIGURING DONE"

:: #############
:: ##  BUILD  ##
:: #############
echo "BUILDING"
cmake --build . --config Debug
echo "BUILDING DONE"
