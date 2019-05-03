# TODO: only add the device spec of the active project
file(GLOB FLEX_SYSTEM_SRCS 
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_specs/*.c
)

# Add the source to the parent project to create a overall library/DLL
set(FLEX_SYSTEM_SRCS
	${FLEX_SYSTEM_SRCS}
	PARENT_SCOPE
)
