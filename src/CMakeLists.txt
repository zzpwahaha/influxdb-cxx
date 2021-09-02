set(INTERNAL_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/src
    )

add_library(InfluxDB-Http OBJECT HTTP.cxx)
target_include_directories(InfluxDB-Http PRIVATE ${INTERNAL_INCLUDE_DIRS})
target_include_directories(InfluxDB-Http SYSTEM PUBLIC $<TARGET_PROPERTY:CURL::libcurl,INTERFACE_INCLUDE_DIRECTORIES>)


add_library(InfluxDB-BoostSupport OBJECT
    $<$<NOT:$<BOOL:${INFLUXCXX_WITH_BOOST}>>:NoBoostSupport.cxx>
    $<$<BOOL:${INFLUXCXX_WITH_BOOST}>:BoostSupport.cxx>
    $<$<BOOL:${INFLUXCXX_WITH_BOOST}>:UDP.cxx>
    $<$<BOOL:${INFLUXCXX_WITH_BOOST}>:UnixSocket.cxx>
    )
target_include_directories(InfluxDB-BoostSupport PRIVATE ${INTERNAL_INCLUDE_DIRS})
target_compile_options(InfluxDB-BoostSupport PRIVATE
       $<TARGET_PROPERTY:date,INTERFACE_COMPILE_OPTIONS>
       )
 target_include_directories(InfluxDB-BoostSupport SYSTEM PRIVATE
     $<TARGET_PROPERTY:date,INTERFACE_INCLUDE_DIRECTORIES>
     )

if (INFLUXCXX_WITH_BOOST)
    target_compile_options(InfluxDB-BoostSupport PRIVATE
        $<TARGET_PROPERTY:Boost::boost,INTERFACE_COMPILE_OPTIONS>
        $<TARGET_PROPERTY:Boost::system,INTERFACE_COMPILE_OPTIONS>
        )
    target_include_directories(InfluxDB-BoostSupport SYSTEM PRIVATE
        $<TARGET_PROPERTY:Boost::boost,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:Boost::system,INTERFACE_INCLUDE_DIRECTORIES>
        )
endif()

add_library(InfluxDB-Internal OBJECT LineProtocol.cxx)
target_include_directories(InfluxDB-Internal PRIVATE ${INTERNAL_INCLUDE_DIRS})


add_library(InfluxDB
    InfluxDB.cxx
    Point.cxx
    InfluxDBFactory.cxx
    $<TARGET_OBJECTS:InfluxDB-Internal>
    $<TARGET_OBJECTS:InfluxDB-Http>
    $<TARGET_OBJECTS:InfluxDB-BoostSupport>
    )
add_library(InfluxData::InfluxDB ALIAS InfluxDB)

generate_export_header(InfluxDB)

target_include_directories(InfluxDB
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    # for export header
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/src>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link targets
target_link_libraries(InfluxDB
  PRIVATE
    CURL::libcurl
    Threads::Threads
    $<$<BOOL:${INFLUXCXX_WITH_BOOST}>:Boost::boost>
    $<$<BOOL:${INFLUXCXX_WITH_BOOST}>:Boost::system>
)

# Use C++17
target_compile_features(InfluxDB PUBLIC cxx_std_${CMAKE_CXX_STANDARD})


# Install library
# CMake < v3.13: install() in the same directory as the target (#98)
install(TARGETS InfluxDB
  EXPORT InfluxDBTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
