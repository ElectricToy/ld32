//
//  ProgrammingArea.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 4/17/15.
//  Copyright (c) 2015 Jeff Wofford. All rights reserved.
//

#include "ProgrammingArea.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( ProgrammingArea )
	
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( ProgrammingArea )
	
	FRESH_DEFINE_CALLBACK( ProgrammingArea, onDragThingDragged, fr::EventTouch )
	{
		auto pos = dragThing().position();
		pos.x = 0;
		
		const auto stageHalfDimensions = stage().stageDimensions().y * 0.5f;
		pos.y = clamp( pos.y, -stageHalfDimensions + 64.0f, stageHalfDimensions - 64.0f );
		dragThing().position( pos );		
	}
	
}

