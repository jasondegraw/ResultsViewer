set(src
  sqlite3.c
  sqlite3.h
)

add_library(sqlite3 STATIC ${src})
if( UNIX )
  target_link_libraries(sqlite3 dl)
endif()

