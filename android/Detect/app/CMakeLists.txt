# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 2.6)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library(Prestissimo SHARED IMPORTED)
set_target_properties(Prestissimo PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../prestissimo/libs/android/${ANDROID_ABI}/libPrestissimo.so)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../prestissimo/include")

add_library(PrestissimoDetectDemo SHARED src/main/cpp/detect.cpp)
target_link_libraries(PrestissimoDetectDemo Prestissimo)


find_library(log-lib log)

target_link_libraries(PrestissimoDetectDemo ${log-lib} jnigraphics)
