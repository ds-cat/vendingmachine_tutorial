set(SERVICE_NAME beverage-preference-service)

add_executable(${SERVICE_NAME}
    BeveragePreferenceService.cpp
)

target_include_directories(${SERVICE_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${THRIFT_GEN_CPP_DIR}
)

target_link_libraries(${SERVICE_NAME}
    ${Boost_LIBRARIES}
    ${THRIFT_LIBRARIES}
    Threads::Threads
    OpenSSL::SSL
    OpenSSL::Crypto
)
