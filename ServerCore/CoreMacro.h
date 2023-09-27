#pragma once


/*------------------------
---------CRASH---------*/

#define CRASH(cause)			\
{								\
	uint32* crash = nullptr;		\
	__analysis_assume(crash != nullptr);		\
	*crash = 0xDEADBEEF;					\
}									

#define ASSERT_CRASH(expr) \
{						\
	if(!(expr))			\
	{					\
		CRASH("Assert_Crash");	\
		__analysis_assume(expr);	\
	}							\
								\
}