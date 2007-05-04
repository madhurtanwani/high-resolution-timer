/*
C++ Wraper (Implementation) over the Timer Library
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

#include "timer_library.h"	/*for using exported fucntions*/
#include "HighResTimerClass.h"


bool HighResTimer::start(double *timeStamp) throw (TimerLibraryWrapperException)
{
	if ( getHandleToLibrary() == NULL ) {
		if ( !initializeLibrary() )
			throw TimerLibraryWrapperException( 
					HIGHRES_TIMER_START_ERROR_IN_INIT_LIB);
	}
	/*success in loading library or lib already loaded!!*/
	ptrToGetTimeStampFunc getTimeStampFunctionPtr = 
			getTimeStampFunctionAddress();
	
	if ( getTimeStampFunctionPtr != NULL ) {
		if ( getTimeStampFunctionPtr(&startTimeStamp) == TIMER_LIB_OK ){
			
			startTimeStamp /= TIMER_LIB_SEC_TO_NANOSEC_MULTIPLIER;
			*timeStamp = startTimeStamp;
			return true;
		}					
		else {
			startTimeStamp = -1.0f;
			*timeStamp = 0.0f;
			return false;
		}
	}
	else {
		throw TimerLibraryWrapperException( 
		HIGHRES_TIMER_ERROR_EXPORTED_FUNC_NOT_FOUND);
	}

return false;
}

bool HighResTimer::stop(double *timeStamp) throw (TimerLibraryWrapperException)
{
	if ( getHandleToLibrary() == NULL ) {
		if ( !initializeLibrary() )
			throw TimerLibraryWrapperException( 
					HIGHRES_TIMER_STOP_ERROR_IN_INIT_LIB);
	}

	/*success in loading library or lib already loaded!!*/
	ptrToGetTimeStampFunc getTimeStampFunctionPtr =  
			getTimeStampFunctionAddress();
				
	if ( getTimeStampFunctionPtr != NULL ) {
		if ( getTimeStampFunctionPtr(&stopTimeStamp) == TIMER_LIB_OK ){		
			
			stopTimeStamp /= TIMER_LIB_SEC_TO_NANOSEC_MULTIPLIER;
			*timeStamp = stopTimeStamp;
			return true;
		}			
		else {
			stopTimeStamp = -1.0f;
			*timeStamp = 0.0f;
			return false;
		}
	}
	else {
		throw TimerLibraryWrapperException( 
		HIGHRES_TIMER_ERROR_EXPORTED_FUNC_NOT_FOUND);
	}

return false;
}

bool HighResTimer::getDifference(double &difference) 
			throw (TimerLibraryWrapperException)
{
	/*if the user has not yet used the library, maybe this is a wrong time
	to call this routine!!!*/
	if ( getHandleToLibrary() == NULL )
		return false;		
		
	/*user has called atleast one of start por stop but not both!!!*/
	if ( startTimeStamp == -1.0f || stopTimeStamp == -1.0f )
		return false;
	else {
		difference = stopTimeStamp - startTimeStamp;
		return true;
	}
		
			
return true;
}

ptrToGetTimeStampFunc HighResTimer::getTimeStampFunctionAddress ()
{
	/*lib not yet loaded!!!*/
	if ( getHandleToLibrary() == NULL )
		return NULL;
	
	ptrToGetTimeStampFunc getTimeStampFunctionPtr;
	
	getTimeStampFunctionPtr = 
			(ptrToGetTimeStampFunc) dlsym (getHandleToLibrary(), 
				HIGHRES_TIMER_LIBRARY_GETTIME_FUNCTION_NAME);

	return getTimeStampFunctionPtr;
}
