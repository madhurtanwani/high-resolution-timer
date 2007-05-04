Timer Library Specifications (overview)

The library is a basic approach to design a base for the performance library. It
has NOT YET taken into consideration the details of library preloads, etc..

	- Designed using "C"
	
	- Uses the available timers on the system and chooses the best from them
	
	- At load time, the library initializes itself as follows :
		- gets the resolution of the timers available on the system by
		averaging the resolutions of each timer in repetitions of 10.
		ire. each timer is queried for its time stamp until there in
		difference in either of its return values (seconds,
		nanoseconds / microseconds), ten times, and the avg resolution
		is then computed. The timer with the least avg resolution is
		chosen as the usable timer. (see initializeTimer and 
		getResolutionOfTimer in timer_library.c).
		
	- at unload time the library cleans up by freeing any allocated memory.
	
	- the library also provides a function initializeSpecificTimer which
	the user can use to initialize a specific timer if he is smart enough to
	find out the best timer. However, the library supports only
	gettimeofday, clock_gettime with REALTIME cock, clock_gettime with
	HIGHRES parameter. (should we be exporting it??)
	
	- library provides a getErrorMessage function to let the user know 
	the detailed report of the error, if any. Format is similar to 
	GetErrorMessage in Win32. The error is internally logged each tier there
	is an error using logErrorMessage. The library outputs either
	TIMER_LIB_OK or TIMER_LIB_FAIED to indicate success or error.
	
	- the library exports 
	Java_HighResTimer_start(JNIEnv *env, jobject this)
	which is the implementation of the native code for start in Java wrapper
	
	- the library exports
	Java_HighResTimer_stop(JNIEnv *env, jobject this)
	which is the implementation of the native code for stop in Java wrapper.
	
	- int getTimeStamp(double *timeStamp) - this function works for the
	start and stop functions by getting the time stamp from the system timer,
	depending on which timer is being used and stores it in the double field
	passed. THIS VALUE IS IN NANO SECONDS and it is the responsibility of
	start and stop to convert it to seconds, if required.

TIMER LIBRARY TEST RESULTS

I used the FileOPen.java test case used in Win32, modified it to include
additional ONLY ONE SIMPLE FILE OPEN check(no directory recursion,nothing -
simple file open) and tested the results on Linux. The following are the
results. The test file was a "kcore" file which amounted to 514MB.

----------------------------------------------------------------------------
./run.sh /windows/f/test/kcore /windows/f/test/
Avg File Open Time = 3.9400000000000544E-5seconds

/windows/f/test/kcore   3.799999999998249E-5    SUCCESS
Average directory parse time = 3.927272727272618E-5seconds
----------------------------------------------------------------------------

Please, if possible, do check that same satisfies what we initially had in mind
and direct me to any mistakes.

TIMER LIBRARY CONCERNS and FUTRTHER WORK DISCUSSION
1.> We needed one library to do the work on both Linux as well as Solaris. I've
done the same :), but here's on concern about the same.
	- Sun compiler "cc"/"CC" DO NOT support the __attribute__ tag to be used
	in its programs. We used the tag to designate the initialization
	functions in the library at load time.( we tell the system that this
	function is the "constructor" so  that when the library loads,this
	function is called.).
	- Instead, "cc" allows us to use #pragma statements and "#pragma init" and
	"#pragma fini" do the trick. However, #pragma statements are IGNORED by
	gcc.
	- So the problem that puts up is that when I try using the library we have
	this conditional code at the start. Is this fine??
		
------------------------------------------------------------------------------
#ifdef __GNUC__		/*the __attribute__ tag is a GNU availability!!*/
	void __attribute__ ((constructor)) timer_library_init();
	void __attribute__ ((destructor)) timer_library_fini();
#else			/*on other systems,say Solaris, we can use pragma!!*/
	#ifdef __sun 	/*using sun systems - so maybe we are using 'cc'*/
		#pragma init (timer_library_init)
		#pragma fini (timer_library_fini)
	#endif
#endif
------------------------------------------------------------------------------

2.> On similar lines, since the two compilers use different compilation flags,
I will need to design a conditional MAKEFILE. I've not yet got done to the
details of that but I hope its possible!!!

3.> Do we really need to worry about library preloads or other issues? One issue
that visibly exists is that, if the library has already been loaded, say using
dlopen or as a shared object due to linking, and another dlopen is called, the
initialization function is NOT called!! (I don't think that's an error since the
getTimeStamp function handles any uninitialized timer existences).

4.> For designing a C++ wrapper on this library, should we be using dlopen or
link the timer library to the wrapper? I've started to design the C++ wrapper
using dlopen, but do tell me if I'm in the wrong direction. The C++ wrapper will
also have three functions, start, stop and getDiffernece analogous to the lava
wrapper and the function getDifference may be passed a double to store the
difference.

