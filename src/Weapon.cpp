//
//  Weapon.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 4/17/15.
//  Copyright (c) 2015 Jeff Wofford. All rights reserved.
//

#include "Weapon.h"
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
	
	// TODO
	
}

