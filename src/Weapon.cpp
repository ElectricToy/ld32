//
//  Weapon.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 4/17/15.
//  Copyright (c) 2015 Jeff Wofford. All rights reserved.
//

#include "Weapon.h"
#include "World.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( Weapon )
	DEFINE_VAR( Weapon, Actor::wptr, m_owner );
	DEFINE_DVAR( Weapon, bool, m_firing );
	DEFINE_DVAR( Weapon, TimeType, m_lastFireTime );
	DEFINE_DVAR( Weapon, TimeType, m_fireDelay );
	DEFINE_VAR( Weapon, std::vector< Shot >, m_shots );
	DEFINE_DVAR( Weapon, size_t, m_currentShot );
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Weapon )
	
	bool Weapon::canFire()
	{
		return world().time() >= m_lastFireTime + m_fireDelay;
	}
	
	void Weapon::reallyFire()
	{
		if( m_shots.empty() == false )
		{
			m_lastFireTime = world().time();
			
			const auto& shot = m_shots[ m_currentShot % m_shots.size() ];
			++m_currentShot;
			
			const ClassInfo::cptr& muzzleFlashClass = std::get< 0 >( shot );
			const vec2& muzzleFlashOffset = std::get< 1 >( shot );
			const ClassInfo::cptr& bulletClass = std::get< 2 >( shot );
			const vec2& bulletOffset = std::get< 3 >( shot );
			
			// Spawn muzzle flash.
			//
			if( muzzleFlashClass )
			{
				auto muzzleFlash = fr::createObject< fr::DisplayObject >( *muzzleFlashClass );
				muzzleFlash->position( owner().position() + muzzleFlashOffset );
				owner().parent()->addChild( muzzleFlash );
			}
			
			// Spawn bullet.
			//
			if( bulletClass )
			{
				auto bullet = fr::createObject< Actor >( *bulletClass );
				bullet->position( owner().position() + bulletOffset );
				owner().parent()->addChild( bullet );
			}
		}
	}
}

