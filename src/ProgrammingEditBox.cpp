//
//  ProgrammingEditBox.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 4/18/15.
//  Copyright (c) 2015 Jeff Wofford. All rights reserved.
//

#include "ProgrammingEditBox.h"
#include "TextField.h"
#include "AppStage.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( ProgrammingEditBox )
	
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( ProgrammingEditBox )
	
	FRESH_DEFINE_CALLBACK( ProgrammingEditBox, onKeyDown, fr::EventKeyboard )
	{
		auto charCode = event.charCode();
		
		// Convert carriage returns to newlines.
		//
		if( charCode == '\r' )
		{
			charCode = '\n';
		}
		
		if( charCode == '\n' && Keyboard::isKeyDown( Keyboard::CtrlCommand ))
		{
			// Compile the program.
			//
			stage().as< AppStage >()->world().loadDroneProgram( text() );
		}
		else
		{
			Super::onKeyDown( event );
		}
	}
}

