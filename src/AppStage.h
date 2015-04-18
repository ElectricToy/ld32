//
//  AppStage.h
//  ld32
//
//  Created by Jeff Wofford on 12/2/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#ifndef ld32_AppStage_h
#define ld32_AppStage_h

#include "Essentials.h"
#include "Stage.h"
#include "World.h"
#include "HUD.h"

namespace ld
{
	
	class AppStage : public fr::Stage
	{
		FRESH_DECLARE_CLASS( AppStage, Stage );
	public:
		
		virtual void update() override
		{
			Super::update();
			
			world().parent()->position( vec2( 0, hud().programmingArea().top() ));
		}
		
		HUD& hud() const
		{
			return getExpectedDescendant< HUD >( *this );
		}
		
		World& world() const
		{
			return getExpectedDescendant< World >( *this, "_world" );
		}
		
	private:
		
	};
	
}

#endif
