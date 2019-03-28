//Use this file to control the amount of RAM used by flexsea-system

#ifndef INC_FLEXSEA_SYSTEM_STACK_CONFIG_H
#define INC_FLEXSEA_SYSTEM_STACK_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

//Note: Rigid and Pocket have to be enabled/disabled here, and in projectsStackConfig.h

#if (defined BOARD_TYPE_FLEXSEA_EXECUTE && !defined BOARD_SUBTYPE_RIGID)

	#define SC_SYS_EN_RI1
	#define SC_SYS_EN_EX1

#elif(defined BOARD_TYPE_FLEXSEA_MANAGE && defined BOARD_SUBTYPE_HABSOLUTE)

	#define SC_SYS_EN_RI1
	#define SC_SYS_EN_EX1

#else

	//Default: everything enabled

	#define SC_SYS_EN_RI1
	#define SC_SYS_EN_RI2
	#define SC_SYS_EN_PCK1
	#define SC_SYS_EN_EX1
	#define SC_SYS_EN_EX2
	#define SC_SYS_EN_EX3
	#define SC_SYS_EN_EX4

#endif

#ifdef __cplusplus
}
#endif

#endif	//INC_FLEXSEA_SYSTEM_STACK_CONFIG_H
