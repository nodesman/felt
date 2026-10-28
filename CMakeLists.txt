cmake_minimum_required(VERSION 3.16)
project(Felt VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_COVERAGE "Enable LLVM source-based code coverage instrumentation" ON)

# Configure Qt 6.5.3 path
set(CMAKE_PREFIX_PATH "/Users/rajsekharan/Qt/6.5.3/macos")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Test)

set(FELT_DOMAIN_SOURCES
    theme.h
    theme.cpp
    themeregistry.h
    themeregistry.cpp
    stats.h
    stats.cpp
    measure.h
    measure.cpp
)

set(FELT_CORE_SOURCES
    ${FELT_DOMAIN_SOURCES}
    highlighter.h
    highlighter.cpp
    editor.h
    editor.cpp
    documenthud.h
    documenthud.cpp
    menubuilder.h
    menubuilder.cpp
    mainwindow.h
    mainwindow.cpp
)

if(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE "Felt.icns")
    set(FELT_ICON_RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/resources/Felt.icns")
    set_source_files_properties(${FELT_ICON_RESOURCE} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
    )
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE
        main.cpp
        ${FELT_CORE_SOURCES}
        ${FELT_ICON_RESOURCE}
        resources/felt.qrc
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_BUNDLE_NAME "Felt"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.nodesman.felt"
        MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
        MACOSX_BUNDLE_ICON_FILE "Felt.icns"
    )
else()
    add_executable(${PROJECT_NAME} main.cpp ${FELT_CORE_SOURCES} resources/felt.qrc)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

# Unit test binary
add_executable(test_felt
    tests/zinsser.h
    tests/test_felt.cpp
    tests/test_core_architecture.cpp
    ${FELT_CORE_SOURCES}
)
target_link_libraries(test_felt PRIVATE Qt6::Widgets Qt6::Test)

if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(test_felt PRIVATE -fprofile-instr-generate -fcoverage-mapping -O0 -g)
    target_link_options(test_felt PRIVATE -fprofile-instr-generate -fcoverage-mapping)
endif()

enable_testing()
add_test(NAME test_felt COMMAND test_felt)
set_tests_properties(test_felt PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
