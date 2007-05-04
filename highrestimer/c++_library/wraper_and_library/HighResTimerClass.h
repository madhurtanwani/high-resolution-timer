/*
C++ Wraper over the Timer Library
Copyright (C) 2005 Madhur Kumar Tanwani

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <time.h>	/*for error logging, if any*/
#include <fstream>

#include <dlfcn.h>
#include <iostream>
using namespace std;

#ifndef _Included_HighResTimerClass
#define _Included_HighResTimerClass

/*the function pointer to the getTimeStamp function*/
typedef int (*ptrToGetTimeStampFunc) (double *);

/*some predefined strings */
#define HIGHRES_TIMER_LIBRARY_NAME "libtimer.so"
#define HIGHRES_TIMER_LIBRARY_GETTIME_FUNCTION_NAME "getTimeStamp"
#define HIGHRES_TIMER_ERROR_FILE_NAME "./errorDump.core"

#define HIGHRES_TIMER_ERROR_UNKNOWN "Unknown Error"
#define HIGHRES_TIMER_START_ERROR_IN_INIT_LIB "start - Error In Initializing Library."
#define HIGHRES_TIMER_STOP_ERROR_IN_INIT_LIB "stop - Error In Initializing Library."
#define HIGHRES_TIMER_ERROR_EXPORTED_FUNC_NOT_FOUND \
	 "Exported function to get time stamp not found in loaded library."

class TimerLibraryWrapperException : exception {
public :
	TimerLibraryWrapperException (char *errorDescrption = NULL ) throw() {
	
		cout << "\n TIMER EXCEPTION...";
		
		ofstream errorOutput(HIGHRES_TIMER_ERROR_FILE_NAME, ios::app);
		if( errorOutput ) {
			char *now = NULL;
			const time_t unbrokenTime = time (NULL);
			now = ctime (&unbrokenTime);
			if ( now ) {
				if ( errorDescrption )
					logError ( errorOutput, now, \
							errorDescrption );
				else
					logError ( errorOutput, now, \
						HIGHRES_TIMER_ERROR_UNKNOWN );
			}
		}
	}		

private:
	void logError ( ofstream &errorStream, char *timeNow, 
				char * errorMessage ) {
		errorStream << timeNow << errorMessage << endl;
	}	
};


class HighResTimer {
public :
	HighResTimer() {
		handleToTimerLibrary = NULL;

		/*dummy init*/
		startTimeStamp = -1.0f;
		stopTimeStamp = -1.0f;
		
		/*will delay the abort process to actual call, if this fails*/
		if ( !initializeLibrary() ) 
			handleToTimerLibrary = NULL;
	} 
	
	~HighResTimer() {
		/*do we need to make a fuss if we are unloading as it is!!*/
		deInitializeLibrary();
	}
	
	bool start(double *timeStamp = NULL) 
			throw (TimerLibraryWrapperException);
	
	bool stop(double *timeStamp = NULL)
			throw (TimerLibraryWrapperException);
			
	bool getDifference(double &difference)
			throw (TimerLibraryWrapperException);

/*internal helper functions*/
private:
	ptrToGetTimeStampFunc getTimeStampFunctionAddress ();
		
	void *getHandleToLibrary() throw() { 
		if ( handleToTimerLibrary == NULL )
			cout <<"\n HANDLE NULL...";
		return handleToTimerLibrary; }

	bool deInitializeLibrary() throw() {
		if ( handleToTimerLibrary != NULL ) {
			if ( dlclose (handleToTimerLibrary) != 0 )
				return false;
			else
				return true;
		}			
	}
	
	bool initializeLibrary() throw() {
		handleToTimerLibrary = dlopen (HIGHRES_TIMER_LIBRARY_NAME,
						RTLD_LAZY);
		if ( handleToTimerLibrary )
			return true;
		else
			return false;
	}
	
private:
	void *handleToTimerLibrary;
	double startTimeStamp;	
	double stopTimeStamp; 
};

#endif	/*_Included_HighResTimerClass*/
