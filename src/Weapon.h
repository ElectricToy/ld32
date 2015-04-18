//
//  Weapon.h
//  FreshApp
//
//  Created by Jeff Wofford on 4/17/15.
//  Copyright (c) 2015 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_Weapon_h
#define FreshApp_Weapon_h

#include "Essentials.h"
#include "Actor.h"

namespace ld
{
	class World;
	
	class Weapon : public fr::Object
	{
		FRESH_DECLARE_CLASS( Weapon, Object );
	public:

		SYNTHESIZE_SET( Actor::wptr, owner );
		
		virtual void update()
		{
			if( m_firing && canFire() )
			{
				fireOnce();
			}
		}
		
		virtual bool canFire();
		virtual void setTrigger( bool trigger )
		{
			m_firing = trigger;
		}
		
		virtual void fireOnce()
		{
			if( canFire() )
			{
				reallyFire();
			}
		}

		Actor& owner() const
		{
			ASSERT( m_owner );
			return *m_owner;
		}
		
		World& world() const
		{
			return owner().world();
		}
		
	protected:
		
		typedef std::tuple< ClassInfo::cptr, vec2, ClassInfo::cptr, vec2 > Shot;
		
		virtual void reallyFire();
		
	private:
		
		VAR( Actor::wptr, m_owner );
		DVAR( bool, m_firing, false );
		DVAR( TimeType, m_lastFireTime, -1.0 );
		DVAR( TimeType, m_fireDelay, 0.25 );
		VAR( std::vector< Shot >, m_shots );
		DVAR( size_t, m_currentShot, 0 );
	};
	
}

#endif
