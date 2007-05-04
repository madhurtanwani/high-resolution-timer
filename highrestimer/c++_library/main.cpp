#include <fcntl.h>
#include <unistd.h>
#include "HighResTimerClass.h"

int main(int argc, char*argv[])
{
	if ( argc != 2)
		return -1;
	
	HighResTimer timer;
	int count = 0;
	double fileOpenTime=0.0, temp=0.0, temp1=0.0, temp2=0.0;

	for ( int i=0 ; i<10 ; i++) {
		timer.start(&temp);
		if ( open (argv[1], O_RDONLY) > 0 ) {
			timer.stop(&temp1);
			timer.getDifference(temp2);
			count++;
		}
		fileOpenTime += temp2;
	}
	
	cout <<"\nAvg FiLE OPEN TIME " << (fileOpenTime/count) << endl;
}
