//
//  Actor.h
//  FreshApp
//
//  Created by Jeff Wofford on 4/17/15.
//  Copyright (c) 2015 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_Actor_h
#define FreshApp_Actor_h

#include "Essentials.h"
#include "World.h"

namespace ld
{
	
	class Actor : public fr::FreshActor
	{
		FRESH_DECLARE_CLASS( Actor, FreshActor );
	public:
		
		World& world() const
		{
			const auto ancestor = fr::firstAncestorOfType< World >( *this );
			ASSERT( ancestor );
			return *ancestor;
		}
		
	private:
		
	};
	
}

#endif
