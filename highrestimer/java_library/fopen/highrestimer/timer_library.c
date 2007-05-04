/*
The Timer Library - uses the system dependant timers / clocks 
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

#include <time.h>		/*for using clock_gettime functions*/
#include <sys/time.h>		/*for gettimeofday function*/
#include <values.h>		/*for MAXDOUBLE*/
#include <stdlib.h>		/*for free and malloc*/
#include <string.h>

#include "timer_library.h"

#ifdef __GNUC__		/*the __attribute__ tag is a GNU availability!!*/
	void __attribute__ ((constructor)) timer_library_init();
	void __attribute__ ((destructor)) timer_library_fini();
#else			/*on other systems,say Solaris, we can use pragma!!*/
	#ifdef __sun 	/*using sun systems - so maybe we are using 'cc'*/
		#pragma init (timer_library_init)
		#pragma fini (timer_library_fini)
	#endif
#endif
	

/*****************************************************************************
LIBRARY STARTUP INIT FUNCTION
It is called automatically when the lib loads and is used here to initialize
the library variables - timer used and errror log.
******************************************************************************/
void timer_library_init()
{
	/*printf ("\n INITILIASING LIBRARY - SETTING TIMER TO USE.\n");*/
	if ( initializeTimer() != TIMER_LIB_OK ) {
		timerUsed = TIMER_LIB_UNKNOWN_TIMER;
		timerResolutionOnThisSystem = MAXDOUBLE;
	}
	logErrorMessage ( TIMER_LIB_OK );	/*no error when i sart, i hope*/
}

/*****************************************************************************
LIBRARY CLEANUP FINISH FUNCTION
It is called automatically when the library unloads. Used here to free memory
******************************************************************************/
void timer_library_fini()
{
	/*printf ("\n LIBRARY UNLOADING - FREEING MEMORY USED, if any\n");*/
	logErrorMessage ( TIMER_LIB_OK );
	/*DONT KNOW IF SHOULD DO THIS OR NOT.... CURRENTLY DISABLING
	because when the same prog used the lib twice init and fini dont get
	called each time!!*/
	/*timerUsed = TIMER_LIB_UNKNOWN_TIMER;
	timerResolutionOnThisSystem = MAXDOUBLE;*/

}

/*****************************************************************************
 FUNCTION NAME    : initializeSpecificTimer
   FUNCTION PURPOSE : to initialze the timer library by selecting the 
		      correct timer typew from those available in the
		      system.
   FUNCTION ARGS    : int timerToUse - it is option usable by power users		       to force the system to use a specific timer,which 
		      may either be USE_REALTIME, USE_HIGHRES or 
		      USE_GETTIMEOFDAY!! In this case the library is NOT
		      responsible for the resolution choice!!
   FUNCION RET VAL :  return processing code - error or success, which
		      may either be TIMER_LIB_OK or TIMER_LIB_FAILED.
		      The "caller" function can call "GetErrorMsg" for
		      knowing more details about the error, if any.
******************************************************************************/
int initializeSpecificTimer( int timerToUse )
{	
	if ( timerToUse == TIMER_LIB_USE_GETTIMEOFDAY ||
		timerToUse == TIMER_LIB_USE_REALTIME ||
		  timerToUse == TIMER_LIB_USE_HIGHRES ) {

		  	logErrorMessage ( TIMER_LIB_OK );	/*no error!!!*/

			/*register timer  to be used*/
			timerUsed = timerToUse ;
			timerResolutionOnThisSystem = 
					getResolutionOfTimer ( timerToUse );
        	        return TIMER_LIB_OK;
		}
	else {
		/*the user has messed up with us - not a supported timer*/
		logErrorMessage (TIMER_LIB_ERROR_INCORRECT_TIMER);
		return TIMER_LIB_FAILED;
	}
}

/*****************************************************************************
FUNCTION NAME    : initializeTimer
   FUNCTION PURPOSE : to initialze the timer library by selecting the
                      correct timer typew from those available in the
                      system.
   FUNCTION ARGS    : NONE - default library provided init routine.
		      Chooses the best timer resource available in sys.
   FUNCION RET VAL :  return processing code - error or success, which
		      may either be TIMER_LIB_OK or TIMER_LIB_FAILED.
		      The "caller" function can call "GetErrorMsg" for
		      knowing more details about the error, if any.
******************************************************************************/
int initializeTimer()
{
/*we will get the system's reslotuion for the three timers
CLOCK_GETTIME using REAL TIME and HIGHRES and GETTIMEOFDAY
then we'll choose the best available.*/
double resolutionOfGetTimeOfDay = getResolutionOfTimer ( \
						TIMER_LIB_USE_GETTIMEOFDAY );
						
double resolutionOfRealtimeClock = getResolutionOfTimer	( \
						TIMER_LIB_USE_REALTIME );
						
double resolutionOfHighResClock = getResolutionOfTimer ( \
						TIMER_LIB_USE_HIGHRES );
			
/*find and use the min resoliution timer*/
/*our preference would be gettimeofday- we've oberved that it
remains fairly constant on the one machine*/
if ( resolutionOfGetTimeOfDay <= resolutionOfRealtimeClock &&
	resolutionOfGetTimeOfDay <= resolutionOfHighResClock &&
	 resolutionOfGetTimeOfDay != MAXDOUBLE ) {
		timerUsed = TIMER_LIB_USE_GETTIMEOFDAY;
		timerResolutionOnThisSystem = resolutionOfGetTimeOfDay;
	
		return TIMER_LIB_OK;
	}

if ( resolutionOfRealtimeClock < resolutionOfGetTimeOfDay &&
	resolutionOfRealtimeClock < resolutionOfHighResClock &&
	 resolutionOfRealtimeClock != MAXDOUBLE ) {
		timerUsed = TIMER_LIB_USE_REALTIME;
		timerResolutionOnThisSystem = resolutionOfRealtimeClock;
		
		return TIMER_LIB_OK;
	}
	
if ( resolutionOfHighResClock != MAXDOUBLE ) {
	timerUsed = TIMER_LIB_USE_HIGHRES;
	timerResolutionOnThisSystem = resolutionOfHighResClock;
	
	return TIMER_LIB_OK;
}
else
	timerUsed = TIMER_LIB_UNKNOWN_TIMER;
	timerResolutionOnThisSystem = MAXDOUBLE;
	logErrorMessage ( TIMER_LIB_ERROR_NO_TIMER_COULD_BE_RESOLVED );	
	return TIMER_LIB_FAILED;
}


/*****************************************************************************
   FUNCTION NAME    : getErrorMessage
   FUNCTION PURPOSE : to report error messages, that are logged, if any.
   FUNCTION ARGS    : char *bufferToHoldErrorMessage - the buffer, to
			hold the error string
		      int lengthOfBuffer - length of the buffer
			including NULL.
   FUNCION RET VAL :  return processing code - error or success, which
		      may either be TIMER_LIB_OK or TIMER_LIB_FAILED.
		      The "caller" function can call "GetErrorMsg" for
		      knowing more details about the error, if any.
******************************************************************************/
int getErrorMessage(char *bufferToHoldErrorMessage, int lengthOfBuffer)
{
	if ( errorLogPtr.errorCode != TIMER_LIB_OK ) {
		if ( errorLogPtr.errorMessage ) {
			if(strlen(errorLogPtr.errorMessage) <= lengthOfBuffer){
				strcpy (bufferToHoldErrorMessage, 
					errorLogPtr.errorMessage);

				return TIMER_LIB_OK;
			}
			else {
				logErrorMessage ( \
					TIMER_LIB_ERROR_BUFFER_TOO_SMALL );

				return TIMER_LIB_FAILED;
			}
		}
		else {
			/*we should be having the error string-is it an error?*/
			strcpy(bufferToHoldErrorMessage, "\0");
			return TIMER_LIB_OK;
		}
	}
	/*no error..... hoooray!!!*/
	strcpy(bufferToHoldErrorMessage, "\0");
	return TIMER_LIB_OK;
}


/*****************************************************************************
   FUNCTION NAME    : getResolutionOfTimer 
   FUNCTION PURPOSE : To get the resolution of the timer on this system
   FUNCTION ARGS    : int timerType - the tier whose resolution to get
   FUNCION RET VAL  : double - the resolution of the timer requested  (nanosec)
******************************************************************************/
double getResolutionOfTimer ( int timerType )
{
	double timerResolution = 0.0;	/*in NANOSECONDS*/
	int counter = 0;
	
	for ( ; counter < TIMER_LIB_AVG_RESOLUTION_REP_COUNT; counter++ ) {
	switch ( timerType )
	{
		case TIMER_LIB_USE_GETTIMEOFDAY : {
		/*we need to check out how fast GETTIMEOFAY ticks!!*/
		struct timeval start_gettime, end_gettime;
	        struct timezone tz;
		
	        if ( gettimeofday(&start_gettime, &tz) != 0 ) {
			logErrorMessage (TIMER_LIB_GETTIMEOFDAY_RETURNED_ERROR);
			return MAXDOUBLE;
	        }
	        do {
	                if ( gettimeofday(&end_gettime, &tz) !=0 ) {
				logErrorMessage ( \
					TIMER_LIB_GETTIMEOFDAY_RETURNED_ERROR);
				return MAXDOUBLE;			
	                }
	        } while ( end_gettime.tv_sec == start_gettime.tv_sec &&
	                        end_gettime.tv_usec == start_gettime.tv_usec );

			timerResolution += \
				(end_gettime.tv_sec - start_gettime.tv_sec) * \
					TIMER_LIB_SEC_TO_NANOSEC_MULTIPLIER + \
				(end_gettime.tv_usec - start_gettime.tv_usec)* \
				       TIMER_LIB_MICROSEC_TO_NANOSEC_MULTIPLIER;

		break;
		}

		case TIMER_LIB_USE_REALTIME : {
		/*we need to check out how fast clock_gettime with 
		 CLOCK_REALTIME parameter ticks!!!*/
		struct timespec start_realtime, end_realtime;

	        if ( clock_gettime ( CLOCK_REALTIME, &start_realtime ) != 0 ) {
			logErrorMessage ( \
			      TIMER_LIB_CLOCK_GETTTIME_REALTIME_RETURNED_ERROR);
			return MAXDOUBLE;			
	        }
	        do {
	                if(clock_gettime(CLOCK_REALTIME, &end_realtime) != 0){
				logErrorMessage ( \
			      TIMER_LIB_CLOCK_GETTTIME_REALTIME_RETURNED_ERROR);
				return MAXDOUBLE;			
	                }
	        } while ( end_realtime.tv_sec == start_realtime.tv_sec &&
	                     end_realtime.tv_nsec == start_realtime.tv_nsec );

			timerResolution += \
				(end_realtime.tv_sec - start_realtime.tv_sec) *\
					TIMER_LIB_SEC_TO_NANOSEC_MULTIPLIER + \
				(end_realtime.tv_nsec - start_realtime.tv_nsec);

		break;
		}

		case TIMER_LIB_USE_HIGHRES : {		
		/*we need to check out how fast clok_gettime with*/
		/* CLOCK_HIGHRES paramter ticks ( only on SOLARIS!!! )*/
	        #ifdef CLOCK_HIGHRES
			struct timespec start_highres, end_highres;
	                if (clock_gettime(CLOCK_HIGHRES, &start_highres) != 0){
				logErrorMessage ( \
			      TIMER_LIB_CLOCK_GETTTIME_HIGHRES_RETURNED_ERROR);
				return MAXDOUBLE;			
		                }
	                do {
	                        if (clock_gettime(CLOCK_HIGHRES, 
							&end_highres )!=0) {
					logErrorMessage ( \
			      TIMER_LIB_CLOCK_GETTTIME_HIGHRES_RETURNED_ERROR);
					return MAXDOUBLE;			
	                        }
	                } while ( end_highres.tv_sec == start_highres.tv_sec &&
	                        end_highres.tv_nsec == start_highres.tv_nsec );
			
			timerResolution += \
				(end_highres.tv_sec - start_highres.tv_sec) * \
					TIMER_LIB_SEC_TO_NANOSEC_MULTIPLIER + \
				(end_highres.tv_nsec - start_highres.tv_nsec);
	
	        #else
			timerResolution = MAXDOUBLE;
	        #endif		
		
		break;
		}
	}
	}	/*for end of 'for'*/
	
	/*so the avg of the values in timerResolution will contain 
	the resolution of the timer*/
	return (timerResolution/TIMER_LIB_AVG_RESOLUTION_REP_COUNT);
}


/*****************************************************************************
   FUNCTION NAME    : logErrorMessage
   FUNCTION PURPOSE : To log error message into the local repos for future use
   FUNCTION ARGS    : errorCode to signify the error so that the descriptive
   			text for the error can be fetched.
   FUNCION RET VAL  : NONE
******************************************************************************/
void logErrorMessage ( int errorCode )
{
	errorLogPtr.errorCode = errorCode;
	
	if ( errorLogPtr.errorMessage != NULL )
		free (errorLogPtr.errorMessage);
	
	if ( errorCode == TIMER_LIB_OK ) {	//no error or cleanup
		if ( errorLogPtr.errorMessage )
				free ( errorLogPtr.errorMessage );
	}
	else {
		errorLogPtr.errorMessage = \
			(char*)(malloc( sizeof(errorMessages[errorCode]) ));
		strcpy(errorLogPtr.errorMessage, errorMessages[errorCode]);
	}
}


/*ACTUAL IMPLEMENTATION OF USEABLE TIMER LIBRARY FUNCTIONS BEGIN*/

/*****************************************************************************
   FUNCTION NAME    :
   FUNCTION PURPOSE :
   FUNCTION ARGS    :
   FUNCION RET VAL  :
******************************************************************************/
JNIEXPORT void JNICALL
Java_highrestimer_HighResTimer_start(JNIEnv *env, jobject this)
{
	double start = 0.0;
	jfieldID fid;
	
	if (getTimeStamp(&start) != TIMER_LIB_OK)
	{
		(*env)->ThrowNew(env, (*env)->FindClass(env, \
			"java/lang/Exception"), \
			"Unable to start timer, high res system function " \
			"returned FALSE");
		return;
	}
	
	/*the value returned by getTimeStamp is in nano sec well convert it*/
	start /= TIMER_LIB_SEC_TO_NANOSEC_MULTIPLIER; /*we are dividing!!*/ 
	fid = (*env)->GetFieldID(env, (*env)->GetObjectClass(env, this),
										"timeDiff", "D");
	(*env)->SetDoubleField(env, this, fid, start);
}
/*****************************************************************************
   FUNCTION NAME    :
   FUNCTION PURPOSE :
   FUNCTION ARGS    :
   FUNCION RET VAL  :
******************************************************************************/
JNIEXPORT void JNICALL
Java_highrestimer_HighResTimer_stop(JNIEnv *env, jobject this)
{
	double stop = 0.0;
	double start = 0.0;
	jfieldID fid;
	
	if (getTimeStamp(&stop) != TIMER_LIB_OK)
	{
		(*env)->ThrowNew(env, (*env)->FindClass(env, \
			"java/lang/Exception"), \
			"Unable to stop timer, high res system function " \
			"returned FALSE");
		return;
	}
	
	/*the value returned by getTimeStamp is in nano sec well convert it*/
	stop /= TIMER_LIB_SEC_TO_NANOSEC_MULTIPLIER; /*we are dividing!!*/ 
	fid = (*env)->GetFieldID(env, (*env)->GetObjectClass(env, this), \
							"timeDiff", "D");
	start = (*env)->GetDoubleField(env, this, fid);
	(*env)->SetDoubleField(env, this, fid, (stop - start));
}

/*****************************************************************************
   FUNCTION NAME    :
   FUNCTION PURPOSE :
   FUNCTION ARGS    :
   FUNCION RET VAL  :
******************************************************************************/
int getTimeStamp(double *timeStamp) 
{
	*timeStamp = MAXDOUBLE;
	
	if ( timerUsed == TIMER_LIB_UNKNOWN_TIMER ) {
	       /*this should not have happened - 
	       however I think we can handle this*/
	       if ( initializeTimer() == TIMER_LIB_FAILED ) {
		       /*no need to log error - its already done*/
		 	*timeStamp = MAXDOUBLE;
			return TIMER_LIB_FAILED;
		}
	}

	switch ( timerUsed )
	{
	case TIMER_LIB_USE_GETTIMEOFDAY : {
		struct timeval gettime_timestamp;
	        struct timezone tz;
		
		
	        if ( gettimeofday(&gettime_timestamp, &tz) != 0 ) {
			logErrorMessage (TIMER_LIB_GETTIMEOFDAY_RETURNED_ERROR);

		 	*timeStamp = MAXDOUBLE;
			return TIMER_LIB_FAILED;
	        }
		*timeStamp = ((gettime_timestamp.tv_sec) * \
				TIMER_LIB_SEC_TO_NANOSEC_MULTIPLIER) + \
				((gettime_timestamp.tv_usec)* \
				TIMER_LIB_MICROSEC_TO_NANOSEC_MULTIPLIER);
		break;
		}

	case TIMER_LIB_USE_REALTIME : {
		struct timespec realtime_timestamp;

		
	        if ( clock_gettime (CLOCK_REALTIME, &realtime_timestamp) != 0){
			logErrorMessage ( \
			      TIMER_LIB_CLOCK_GETTTIME_REALTIME_RETURNED_ERROR);

		 	*timeStamp = MAXDOUBLE;
			return TIMER_LIB_FAILED;
	        }
		*timeStamp = ((realtime_timestamp.tv_sec) * \
					TIMER_LIB_SEC_TO_NANOSEC_MULTIPLIER) + \
					(realtime_timestamp.tv_nsec);

		break;
		}

	case TIMER_LIB_USE_HIGHRES : {
		/*we need to check out how fast clok_gettime with*/
		/* CLOCK_HIGHRES paramter ticks ( only on SOLARIS!!! )*/
	        #ifdef CLOCK_HIGHRES		
		struct timespec highres_timestamp;
		
               if (clock_gettime(CLOCK_HIGHRES, &highres_timestamp) != 0){
			logErrorMessage ( \
			    TIMER_LIB_CLOCK_GETTTIME_HIGHRES_RETURNED_ERROR);

		 	*timeStamp = MAXDOUBLE;
			return TIMER_LIB_FAILED;
	        }
		*timeStamp = ((highres_timestamp.tv_sec) * \
				TIMER_LIB_SEC_TO_NANOSEC_MULTIPLIER) + \
				(highres_timestamp.tv_nsec);

	        #else
			*timeStamp = MAXDOUBLE;
			
			logErrorMessage ( \
			 TIMER_LIB_ERROR_UNSUPPORTED_HIGHRES_TIMESTAMP_REQUEST);
			
			return TIMER_LIB_FAILED;
	        #endif
		
		break;
		}
	}

	if ( *timeStamp != MAXDOUBLE )
		return TIMER_LIB_OK;
	else
		return TIMER_LIB_FAILED;
}
