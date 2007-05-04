echo "Compiling Shared Timer Library."
	gcc -shared -fPIC timer_library.c -lrt -o libtimer.so
if [ $? -ne 0 ]
then
	echo "Error in Compieling Timer ibrary. Quiting..."
	exit 0
fi

echo "Compiling C++ Wrapper for Timer Library."
	c++ HighResTimerClass.cpp -shared -ldl -o libHighResTimer.so
if [ $? -ne 0 ]
then
	echo "Error in Compiling C++ Wrapper for Timer Library. Quiting..."
	exit 0
fi
echo "Done."
