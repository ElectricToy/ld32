//
//  Actor.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 4/17/15.
//  Copyright (c) 2015 Jeff Wofford. All rights reserved.
//

#include "Actor.h"
#include "World.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( Actor )
	DEFINE_VAR( Actor, ClassInfo::cptr, m_shadowHostClass );
	DEFINE_VAR( Actor, fr::DisplayObjectContainer::ptr, m_shadowHost );
	DEFINE_DVAR( Actor, vec2, m_shadowOffset );
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Actor )
	
	World& Actor::world() const
	{
		const auto ancestor = fr::firstAncestorOfType< World >( *this );
		ASSERT( ancestor );
		return *ancestor;
	}
	
	void Actor::update()
	{
		Super::update();
		
		if( m_shadowHost )
		{
			if( !m_shadowHost->parent() )
			{
				world().shadowLand().addChild( m_shadowHost );
			}
			
			m_shadowHost->position( position() + m_shadowOffset );
			
			if( m_shadowHost->numChildren() > 0 )
			{
				if( auto shadow = m_shadowHost->getChildAt( 0 )->as< Sprite >() )
				{
					shadow->blendMode( fr::Renderer::BlendMode::Alpha );
					shadow->texture( texture() );
					shadow->pivot( pivot() );
					shadow->scale( scale() );
				}
			}
		}
	}
}

