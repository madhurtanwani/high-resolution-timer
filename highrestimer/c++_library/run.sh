if [ $# -eq 1 ]
then
	#we will first look at compiling the library
	system=$("uname")
	if [ $system == "SunOS" ]
	then
		compile_script="./compile_solaris.sh"
	else
		compile_script="./compile_linux.sh"
	fi
	
	#exec the compilation script for the library and java wrapper
	dir=wraper_and_library
	cd $dir
	$compile_script	
	cd ../
	
	echo "Compiling C++ FileOPen Test Program."
	export LD_LIBRARY_PATH=.:./$dir
	c++ main.cpp -I$dir -L$dir -lHighResTimer -o test_timer_lib.o
	./test_timer_lib.o "$1"
else
	echo "Usage : ./run.sh <FILE-PATH>"
fi
