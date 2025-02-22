add_library(way argparser.cpp routes.cpp cache.cpp)

target_include_directories(routes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_package(nlohmann_json REQUIRED)
find_package(cpr REQUIRED)
find_package(OpenSSL REQUIRED)

target_link_libraries(routes 
    PRIVATE 
        nlohmann_json::nlohmann_json 
        cpr::cpr 
        OpenSSL::SSL 
        OpenSSL::Crypto
)   