add_library(
  VideoProcessor SHARED
  include/VideoProcessor.h
  src/VideoProcessor.cpp
  include/Frame.h
  src/Frame.cpp
  include/Corrector.h
  src/Corrector.cpp
  include/Neighbor.h
  src/Neighbor.cpp
)

target_include_directories( VideoProcessor PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

