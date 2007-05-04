echo "Compiling Shared Timer Library."
	echo "Please enter the path of the include directory of JAVA installation (without traiing '/' "

	read include_path
	include_path1="$include_path/solaris"
	
	cc -G -xcode=pic32 timer_library.c -lrt -I$include_path -I$include_path1 -o libtimer.so
if [ $? -ne 0 ]
then
	echo "Error in Compieling Timer ibrary. Quiting..."
	exit 0
fi

echo "Compiling Java Wrapper for Timer Library."
	javac HighResTimer.java
if [ $? -ne 0 ]
then
	echo "Error in Compiling Java Wrapper for Timer Library. Quiting..."
	exit 0
fi
echo "Done."
