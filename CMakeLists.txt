# On plan we want to pull in the device spec conglomerator to know
# what type of data all the devices have
if(FLEXSEA_TYPE STREQUAL PLAN)
	file(GLOB FLEX_SYSTEM_SRCS
		${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
		${CMAKE_CURRENT_SOURCE_DIR}/device_specs/plan_spec_conglomerator.c
	)
# on manage we only want to use the device spec file for the current project
else()
	file(GLOB FLEX_SYSTEM_SRCS
		${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
		${CMAKE_CURRENT_SOURCE_DIR}/device_specs/${APP_TYPE}_device_spec.c
	)
endif()

# Add the source to the parent project to create a overall library/DLL
set(FLEX_SYSTEM_SRCS
	${FLEX_SYSTEM_SRCS}
	PARENT_SCOPE
)
