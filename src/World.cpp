//
//  World.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 4/17/15.
//  Copyright (c) 2015 The Electric Toy Co. All rights reserved.
//

#include "World.h"
#include "Camera.h"
#include "DroneController.h"

namespace ld
{
	FRESH_DEFINE_CLASS( World )
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( World )
	
	void World::loadDroneProgram( const std::string& programText )
	{
		eachDrone( [&]( Drone& drone )
				  {
					  auto droneController = drone.controller()->as< DroneController >();
					  droneController->loadProgramText( programText );
				  } );
	}
}