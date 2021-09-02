add_executable(InfluxDBST InfluxDBST.cxx $<TARGET_OBJECTS:InfluxDB-Http>)
target_link_libraries(InfluxDBST PRIVATE InfluxDB CURL::libcurl TestMain)
target_include_directories(InfluxDBST PRIVATE ${PROJECT_SOURCE_DIR}/src)


add_custom_target(systemtest InfluxDBST
        COMMENT "Running system tests\n\n"
        VERBATIM
        )
