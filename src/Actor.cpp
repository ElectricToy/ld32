//
//  Actor.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 4/17/15.
//  Copyright (c) 2015 Jeff Wofford. All rights reserved.
//

#include "Actor.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( Actor )
	DEFINE_VAR( Actor, ClassInfo::cptr, m_shadowHostClass );
	DEFINE_VAR( Actor, fr::DisplayObjectContainer::ptr, m_shadowHost );
	DEFINE_DVAR( Actor, vec2, m_shadowOffset );
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Actor )
	
	// TODO
	
}

