project(qwt)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Svg REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5Concurrent REQUIRED)
find_package(Qt5PrintSupport REQUIRED)

set(QT_LIBS Qt5::Core Qt5::Gui Qt5::Svg Qt5::Xml
  Qt5::Concurrent Qt5::PrintSupport) 

add_subdirectory(src)
