//
//  Drone.h
//  FreshApp
//
//  Created by Jeff Wofford on 4/17/15.
//  Copyright (c) 2015 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_Drone_h
#define FreshApp_Drone_h

#include "Essentials.h"
#include "Pawn.h"

namespace ld
{
	
	class Drone : public Pawn
	{
		FRESH_DECLARE_CLASS( Drone, Pawn );
	public:

		virtual void update() override
		{
			Super::update();
			
			// Clamp to sides.
			//
			const auto PADDING = 8.0f;
			const auto halfDimensions = dimensions() * 0.5f;
			
			const auto pos = position();
			position( clamp( pos.x, WORLD_RANGE_X.min + halfDimensions.x + PADDING, WORLD_RANGE_X.max - halfDimensions.x - PADDING ), pos.y );
		}
	};
	
}

#endif
