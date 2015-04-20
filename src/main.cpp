//
//  main.cpp
//
//  Created by Jeff Wofford on 2014/03/04.
//  Copyright 2014 jeffwofford.com. All rights reserved.
//

#include "ApplicationStaged.h"
#include "FreshFile.h"
#include "ParticleEmitter.h"
using namespace fr;

namespace
{
	template< typename T >
	void use()
	{
		typename T::ptr t = createObject< T >();
	}
}

namespace ld
{
	void forceInclusion()
	{
		// Reference Fresh static library functions and classes that might otherwise be stripped due to apparent non-use.
		use< fr::ParticleEmitter >();
	}
}

int main( int argc, char* argv[] )
{
	ApplicationStaged app( "assets/config.fresh" );
	int retVal = app.runMainLoop( argc, argv );
	
    return retVal;
}
