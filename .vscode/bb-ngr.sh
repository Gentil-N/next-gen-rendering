echo "------------------------------Building"

if [ "$1" = "debug" ]
then
       echo "------------------------------Debug"
       mkdir -p build/debug
       cd build/debug
       cmake ../../. -DCMAKE_BUILD_TYPE="Debug" -DNGR_BUILD_SHARED=OFF
elif [ "$1" = "release" ]
then
       echo "------------------------------Release"
       mkdir -p build/release
       cd build/release
       cmake ../../. -DCMAKE_BUILD_TYPE="Release" -DNGR_BUILD_SHARED=OFF
else
       echo "build type is not valid"
       exit 1
fi

make -j4

echo "------------------------------Done"