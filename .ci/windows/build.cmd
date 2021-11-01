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

echo "BUILDING DONE"
