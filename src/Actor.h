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

namespace ld
{
	class World;
	
	class Actor : public fr::FreshActor
	{
		FRESH_DECLARE_CLASS( Actor, FreshActor );
	public:
		
		World& world() const;
		
		virtual void update() override;
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
