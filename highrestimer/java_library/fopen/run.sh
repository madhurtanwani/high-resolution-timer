if [ $# -eq 2 ]
then
	#we will first look at compiling the library
	system=$("uname")
	if [ $system == "SunOS" ]
	then
		compile_script="./compile_solaris.sh"
	else
		compile_script="./compile_linux.sh"
	fi
	
	#exec the compileation script for the library and java wrapper
	cd ./highrestimer/
	$compile_script	
	cd ../
	
	echo "Compiling and Executing Java FileOpen Class."		
	javac FileOpen.java
	dir=$("pwd")
	dir="$dir/highrestimer/:$PATH"
	java "-Djava.library.path=$dir" FileOpen "$1" "$2"
else
	echo "Usage : ./run.sh <FILE-PATH> <DIRECTORY-PATH>"
fi
