//
//  Pawn.h
//  FreshApp
//
//  Created by Jeff Wofford on 4/18/15.
//  Copyright (c) 2015 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_Pawn_h
#define FreshApp_Pawn_h

#include "Essentials.h"
#include "Actor.h"
#include "Weapon.h"

namespace ld
{
	
	class Pawn : public Actor
	{
		FRESH_DECLARE_CLASS( Pawn, Actor );
	public:
		
		virtual void update() override
		{
			Super::update();
			
			for( auto weapon : m_weapons )
			{
				weapon->owner( this );
				weapon->update();
			}
		}
		
		bool canFireWeapon( size_t i ) const
		{
			return m_weapons.size() > i;
		}
		
		void setTrigger( size_t i, bool trigger )
		{
			if( m_weapons.size() > i )
			{
				m_weapons[ i ]->setTrigger( trigger );
			}
		}
		
		void fireOnce( size_t i )
		{
			if( m_weapons.size() > i )
			{
				m_weapons[ i ]->fireOnce();
			}
		}
		
		// TODO
		virtual void jump( real power ) override
		{
			fireOnce( 0 );
		}
		
	private:
		
		VAR( std::vector< Weapon::ptr >, m_weapons );
		
	};
	
}

#endif
