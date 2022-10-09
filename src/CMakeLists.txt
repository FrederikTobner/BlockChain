set(LIBARY_SOURCES
sha256.c
block.c
blockchain.c
)

set(LIBARY_HEADERS
sha256.h
block.h
blockchain.h
)

add_library(${PROJECT_NAME} ${LIBARY_HEADERS} ${LIBARY_SOURCES})

# Precompiles includes from the c-standard-libary to speed up compilation of the target
if( MSVC )    
    target_precompile_headers(${PROJECT_NAME} PUBLIC precompiled_header.h precompiled_header.c) # MSVC only accepts header files that also have a source file
else()
    target_precompile_headers(${PROJECT_NAME} PUBLIC precompiled_header.h)
endif()