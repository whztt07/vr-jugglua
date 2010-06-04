/**	@file	NavInteractive.cpp
	@brief	implementation

	@date
	2009-2010

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

// Internal Includes
#include "FLTKNav.h"

// Library/third-party includes
#include <vrj/Kernel/Kernel.h>

// Standard includes
#include <iostream>

using namespace vrjLua;

int main(int argc, char * argv[]) {
	vrj::Kernel::setUseCocoaWrapper(false);
	FLTKNav nav;

	return nav.run();

	//return 0;
}
