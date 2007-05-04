echo "Compiling Shared Timer Library."
	gcc -shared -fPIC timer_library.c -lrt -o libtimer.so
if [ $? -ne 0 ]
then
	echo "Error in Compieling Timer ibrary. Quiting..."
	exit 0
fi

echo "Compiling Java Wrapper for Timer Library."
	javac HighResTimer.java
if [ $? -ne 0 ]
then
	echo "Error in Compiling Java Wrapper for Timer ibrary. Quiting..."
	exit 0
fi
echo "Done."
