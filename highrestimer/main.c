/*
Example Driver Class To Demonstrate The Time Library Usage
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

#include <dlfcn.h>
#include <stdio.h>
#include <limits.h>
#include <values.h>

/*return codes - error numbers and success code*/
#include "timer_library_return_codes.h"

/* variables and functins fro the timer library*/
extern int timerUsed;
extern int getTimerUsed();
void *lib_handle, *lib_handle1;

int main()
{
	int timerUsed1;// = getTimerUsed();
	int retval;
	double timeStamp, timeStamp1;

	retval=initializeTimer();
	printf("\n INIT RET CODE = %d", retval);
	if ( retval == TIMER_LIB_OK )
		printf("\n TIMER USED = %d", timerUsed);

	retval = getTimeStamp(&timeStamp);
	if ( retval == TIMER_LIB_OK )
		printf("\n TIMESTAMP = %g", timeStamp);
	else
		printf("\n ERROR IN getTimeStamp");

	retval = getTimeStamp(&timeStamp1);
	if ( retval == TIMER_LIB_OK )
		printf("\n TIMESTAMP = %g", timeStamp1);
	else
		printf("\n ERROR IN getTimeStamp");

	printf("\n DIFFERENCE = %g microseconds", (timeStamp1-timeStamp)/1000);


	getTimeStamp(&timeStamp);
	getTimeStamp(&timeStamp1);
	printf("\n IMMEDIATE DIFFERENCE = %g microseconds", (timeStamp1-timeStamp)/1000);
	

/*	printf("\n USING DLOPEN");
	lib_handle = dlopen("./libtimer.so", RTLD_LAZY);
	if (!lib_handle) {
 	   printf("Error during dlopen(): %s\n", dlerror());
	   exit(1);
	}
	//timerUsed1 = getTimerUsed();
	//dlclose(lib_handle); 	
	printf("\n Timer Used = %d", timerUsed1 );
	printf("\nDLOPEN DONE..");

        printf("\n USING DLOPEN");
        lib_handle1 = dlopen("./libtimer.so", RTLD_LAZY);
        if (!lib_handle1) {
           printf("Error during dlopen(): %s\n", dlerror());
           exit(1);
        }
        //timerUsed1 = getTimerUsed();
        dlclose(lib_handle1);
        printf("\n Timer Used = %d", timerUsed1 );
        printf("\nDLOPEN DONE..");

	dlclose(lib_handle); 	*/
printf("\n\n");
}
