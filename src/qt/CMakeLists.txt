# Set up QT4 and required components
find_package(Qt5Core)
find_package(Qt5Widgets CONFIG REQUIRED)

include_directories(Qt5Widgets_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/.. .)

###
# All the project files
set(SOURCES
     main.cpp
     mainwindow.cpp
     )

#set(HEADERS)

set(MOCS mainwindow.h)

set(UIS mainwindow.ui)

set(RESOURCES imgs.qrc)

###
# Build and link the project

# Let QT pre-process the files and add the generated files to the source list
qt5_wrap_ui(GENERATED_SOURCES ${UIS})
qt5_add_resources(GENERATED_SOURCES ${RESOURCES})
qt5_wrap_cpp(GENERATED_SOURCES ${MOCS})

#source_group("Generated Sources - Do Not Edit" FILES ${GENERATED_SOURCES})

# Make sure the compiler can find the pre-processed files from qt5_wrap_ui

# Build the app!
add_executable(Shamir-qt
     ${SOURCES}
     ${GENERATED_SOURCES})

target_link_libraries(Shamir-qt Qt5::Widgets Qt5::Core shamirsharing)
