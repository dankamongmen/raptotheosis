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

###################### USER-SELECTABLE OPTIONS ###########################
option(USE_PANDOC "Build man pages with pandoc" ON)
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release MinSizeRel RelWithDebInfo Coverage)
############## END (additional) USER-SELECTABLE OPTIONS ##################

# global compiler flags
add_compile_definitions(FORTIFY_SOURCE=2)
add_compile_options(-Wall -Wextra -W -Wshadow -Wformat -fexceptions)

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
    "${Notcurses_INCLUDE_DIRS}"
  )
target_link_directories(raptotheosis
  PRIVATE
    "${Notcurses_LIBRARY_DIRS}"
  )
target_link_libraries(raptotheosis
  PRIVATE
    "${Notcurses_LIBRARIES}"
  )

# Pandoc documentation (man pages, HTML reference)
if(USE_PANDOC)
  file(GLOB MANSOURCE1 CONFIGURE_DEPENDS doc/*1.md)
  find_program(PANDOC pandoc)
  if(NOT PANDOC)
    message(FATAL_ERROR "pandoc not found. USE_PANDOC=OFF to disable.")
  else()
    foreach(m ${MANSOURCE1})
      get_filename_component(me ${m} NAME_WLE)
      add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${me}
        DEPENDS ${m}
        COMMAND ${PANDOC}
        ARGS --to man --standalone --from markdown-smart ${m} > ${CMAKE_CURRENT_BINARY_DIR}/${me}
        COMMENT "Building man page ${me}"
      )
      add_custom_target(${me}.man
        ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${me}
      )
    endforeach()
    foreach(m ${MANSOURCE1})
      get_filename_component(me ${m} NAME_WLE)
      LIST(APPEND MANPAGES1 ${CMAKE_CURRENT_BINARY_DIR}/${me})
    endforeach()
  endif()
endif()

install(TARGETS raptotheosis DESTINATION bin)
install(FILES ${MANPAGES1} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/man/man1)
