cmake_minimum_required(VERSION 2.8)
project( CorruptedToCorrected )


# OpenCV include
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# VideoProcessor included as library
add_subdirectory( VideoProcessor )

# Executable
add_executable( CorruptedToCorrected src/CorruptedToCorrected.cpp )

# Linking the executable
target_link_libraries( CorruptedToCorrected PUBLIC ${OpenCV_LIBS} VideoProcessor)

# Tester
ENABLE_TESTING()
ADD_EXECUTABLE( UnitTests src/UnitTests.cpp )
target_link_libraries( UnitTests PUBLIC ${OpenCV_LIBS} VideoProcessor)
ADD_TEST( VideoProcessorTest UnitTests )
