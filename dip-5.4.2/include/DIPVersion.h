#ifndef DIPVERSION_H
#define DIPVERSION_H

#include "Options.h"
#include <assert.h>
#include <string>

class DipVersion{
public:

/**
*	Get the version string from the version of DIP being used.
*/
static const std::string & getDipVersion();

/**
* Fixed len - must NEVER change, Al DIMDIP type services will
* rely on this.
*/
static unsigned int getDipVersionStringLen(){
	return 9;
} 
};

#endif
