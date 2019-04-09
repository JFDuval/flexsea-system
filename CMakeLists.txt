file(GLOB FLEX_SYSTEM_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

# Add the source to the parent project to create a overall library/DLL
set(FLEX_SYSTEM_SRCS
	${FLEX_SYSTEM_SRCS}
	PARENT_SCOPE
)
