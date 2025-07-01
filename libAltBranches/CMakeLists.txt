cmake_minimum_required(VERSION 3.10)
project(libAltBranches VERSION 0.1.0 LANGUAGES C CXX)

add_library(AltBranches SHARED libAltBranches.cpp)

find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(RPM REQUIRED rpm)


include_directories(${RPM_INCLUDE_DIRS})

target_include_directories(AltBranches
    PUBLIC
        ${PROJECT_SOURCE_DIR}         
        ${CURL_INCLUDE_DIRS}
)


target_link_libraries(AltBranches
    PUBLIC
        ${CURL_LIBRARIES}
    PRIVATE 
        nlohmann_json::nlohmann_json
        rpm
)



install(TARGETS AltBranches DESTINATION lib)
install(FILES libAltBranches.h DESTINATION include)