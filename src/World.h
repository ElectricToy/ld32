//
//  World.h
//  FreshApp
//
//  Created by Jeff Wofford on 4/17/15.
//  Copyright (c) 2015 The Electric Toy Co. All rights reserved.
//

#ifndef __FreshApp__World__
#define __FreshApp__World__

#include "Essentials.h"
#include "TimeServer.h"
#include "Drone.h"

namespace ld
{
	class World : public fr::FreshWorld, public fr::TimeServer
	{
		FRESH_DECLARE_CLASS( World, FreshWorld )
	public:
		
		virtual TimeType time() const
		{
			return nUpdates() * 1.0 / 60.0;
		}

		fr::DisplayObjectContainer& shadowLand() const
		{
			return getExpectedDescendant< DisplayObjectContainer >( *this, "_shadowLand" );
		}
		
		void loadDroneProgram( const std::string& programText );
		
		template< typename FunctionT >
		void eachDrone( FunctionT&& fn )
		{
			forEachDescendant< Drone >( std::move( fn ));
		}
	};
}
#endif /* defined(__FreshApp__World__) */
