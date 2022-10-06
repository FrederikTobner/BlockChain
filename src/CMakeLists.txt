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