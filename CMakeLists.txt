cmake_minimum_required(VERSION 3.14.0)
project(raptotheosis VERSION 0.0.1
  DESCRIPTION "Cross-distro package manager"
  HOMEPAGE_URL "https://nick-black.com/dankwiki/index.php?title=Raptorial"
  LANGUAGES C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_VISIBILITY_PRESET hidden)

include(CTest)
include(GNUInstallDirs)
include(FeatureSummary)

#find_package(Threads)
#set_package_properties(Threads PROPERTIES TYPE REQUIRED)
find_package(Notcurses 2.2.2 CONFIG)
set_package_properties(Notcurses PROPERTIES TYPE REQUIRED)
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

include(CMakePackageConfigHelpers)
configure_file(tools/version.h.in include/version.h)

file(GLOB SRCS CONFIGURE_DEPENDS src/*.c)
add_executable(raptotheosis ${SRCS})
target_include_directories(raptotheosis
  PRIVATE
    "${PROJECT_BINARY_DIR}/include"
)

install(TARGETS raptotheosis DESTINATION bin)
