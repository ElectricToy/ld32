//
//  DroneController.h
//  FreshApp
//
//  Created by Jeff Wofford on 4/17/15.
//  Copyright (c) 2015 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_DroneController_h
#define FreshApp_DroneController_h

#include "Essentials.h"
#include "FreshPlayerController.h"
#include "Drone.h"

namespace ld
{
	
	class DroneController : public fr::FreshPlayerController
	{
		FRESH_DECLARE_CLASS( DroneController, FreshPlayerController );
	public:
		
		virtual void update() override
		{
			Super::update();

//			const real steerDir = 1;
//			
//			drone().applyControllerImpulse( vec2( sign( steerDir ), 0 ));
		}
		
		Drone& drone() const
		{
			return static_cast< Drone& >( *host() );
		}
		
	private:
		
	};
	
}

#endif
