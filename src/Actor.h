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
		
		virtual void update()
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
		
		virtual void onAddedToStage()
		{
			Super::onAddedToStage();
			createShadow();
		}
		
	protected:
		
		void createShadow()
		{
			if( !m_shadowHost && m_shadowHostClass )
			{
				m_shadowHost = fr::createObject< DisplayObjectContainer >( *m_shadowHostClass );
			}
			
			if( m_shadowHost )
			{
				m_shadowHost->removeChildren();
				m_shadowHost->addChild( fr::createObject< fr::Sprite >() );
			}
		}
		
	private:
		
		VAR( ClassInfo::cptr, m_shadowHostClass );
		VAR( fr::DisplayObjectContainer::ptr, m_shadowHost );
		DVAR( vec2, m_shadowOffset, vec2( 16 ));
		
	};
	
}

#endif
