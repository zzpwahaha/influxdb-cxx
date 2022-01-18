# step-by-step walk in for building InfluxDB on windows 
by Zhenpu Zhang @ 01/17/2022

## install dependency
For this build, without testing we just need ***boost*** as well as ***curl***. And if we want to test it after build, we would need ***catch2*** and ***trompeloeil*** further.

-  ***Boost***:
   1. To monitor the process, we added `set(Boost_DEBUG ON)` in the CMakeLists.txt 
   2. and it turns out we need a complete build of boost with both .dll and .lib in folder boost1_17_0\stage\lib. To get this, we need to run `b2  --build-type=complete stage` after bootstraping boost.
   3. Furthermore, to make sure the makefile can find boost, we need to add 
    ```c++
        set(BOOST_ROOT "C:/Software/Boost/boost_1_76_0")
        set(BOOST_INCLUDEDIR  "C:/Software/Boost/boost_1_76_0")
        set(BOOST_LIBRARYDIR "C:/Software/Boost/boost_1_76_0/stage/lib")
    ```
    in the CMakeLists.
-  ***curl***: 
    Download vcpkg from [their website](https://vcpkg.io/en/getting-started.html) and install to C:/Software/vcpkg. Then install the required library with the following command:
    ```
    vcpkg install curl:x64-windows --x-install-root=C:\Chimera\influxdb-cxx\3rd-party
    ``` 
    Keep in mind that the libcurl.lib file need to be included in the MSVC project if one want to use the influxdb.h later as an external library.
     
- ***catch2***, ***trompeloeil***: 
    same as ***curl***, the only difference is that these two are not needed when use the influxDB. And to make sure the makefile can find these three packages, add 
    ```c++
    list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/3rd-party/x64-windows")
    ```
    to the CMakeLists.txt file.
  
## Note to take care of
1. Do **NOT** check BUILD_SHARED_LIBS option. That will cause the build of the vcproject not working and no .lib file generated.
2. Some of the test requires boost dependence and one need to add the boost libraries directory in the influx-cxx.sln project
3. One can run the test file at influxdb-cxx\build\test\Release. Only BoostSupportTest and HttpTest are not all passing since they need to connect to server which is not quite defined under test.


