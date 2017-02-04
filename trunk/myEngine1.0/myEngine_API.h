#ifndef MYENGINE_API_H
#define MYENGINE_API_H
	#ifdef MYENGINE_EXPORTS
		#define MYENGINE_API __declspec(dllexport)
	#else
		#define MYENGINE_API __declspec(dllimport)
	#endif
#endif