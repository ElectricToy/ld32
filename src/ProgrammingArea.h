//
//  ProgrammingArea.h
//  FreshApp
//
//  Created by Jeff Wofford on 4/17/15.
//  Copyright (c) 2015 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_ProgrammingArea_h
#define FreshApp_ProgrammingArea_h

#include "Essentials.h"

namespace ld
{
	
	class ProgrammingArea : public fr::UIPopup
	{
		FRESH_DECLARE_CLASS( ProgrammingArea, UIPopup );
	public:
		
		fr::Sprite& dragThing() const
		{
			return getExpectedDescendant< fr::Sprite >( *parent(), "_dragThing" );
		}
		
		fr::Sprite& background() const
		{
			return getExpectedDescendant< fr::Sprite >( *this, "_background" );
		}
		
		virtual void update() override
		{
			Super::update();
			
			const auto& stageBounds = stage().stageBounds();
			const auto borderPos = dragThing().position();
			
			const auto elevation = stageBounds.bottom() - borderPos.y;
			const auto midpoint = elevation * 0.5f;
			
			background().position( 0, stageBounds.bottom() - midpoint );
			background().scale( 160.0f, elevation * 0.5f );
		}
		
		real top() const
		{
			return dragThing().bounds().top();
		}
		
		virtual void onAddedToStage() override
		{
			Super::onAddedToStage();
			
			dragThing().addEventListener( DRAG_MOVE, FRESH_CALLBACK( onDragThingDragged ));
		}
		
	private:
		
		FRESH_DECLARE_CALLBACK( ProgrammingArea, onDragThingDragged, fr::EventTouch )
	};
	
}

#endif
