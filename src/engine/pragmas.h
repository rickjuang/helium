/*************************************************************
 *	Pragmas
 *	by Rick Juang, 2011/06/20
 *	Compiler directives
 *************************************************************/

#ifdef _WIN32
	// turn these warnings off
	//#pragma warning(disable: 4100)	// unreferenced formal parameter
	//#pragma warning(disable: 4311)	// stupid 64-bit pointer warning which shouldn't be firing anyway
	//#pragma warning(disable: 4312)	// stupid 64-bit pointer warning which shouldn't be firing anyway
	//#pragma warning(disable: 4355)	// "this" : used in base member initializer list
	//#pragma warning(disable: 4512)	// assignment operator could not be generated
	#pragma warning(disable: 4996)	// "strcpy", "sprintf", etc was declared deprecated

	// turn these warnings on
	#pragma warning(3: 4189)			// local variable initialized but not referenced
	#pragma warning(3: 4555)			// expression has no effect
	#pragma warning(3: 4701)			// local variable 'name' may be used without having been initialized
	#pragma warning(3: 4702)			// unreachable code
	#pragma warning(3: 4709)			// comma operator within array index expression
#endif
