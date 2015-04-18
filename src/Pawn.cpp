//
//  Pawn.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 4/18/15.
//  Copyright (c) 2015 Jeff Wofford. All rights reserved.
//

#include "Pawn.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( Pawn )
	DEFINE_VAR( Pawn, std::vector< Weapon::ptr >, m_weapons );
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Pawn )
	
	// TODO
	
}

