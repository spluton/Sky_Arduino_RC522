#pragma once

#include <stdlib.h>
#include <stdio.h>



class Checksum {

public:

	unsigned short ComputeCcittCrc16(void const *, unsigned int );

protected:
	
	 unsigned short UpdateCcittCrc16(unsigned short , unsigned char );

};
