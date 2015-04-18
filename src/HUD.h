//
//  HUD.h
//  FreshApp
//
//  Created by Jeff Wofford on 4/17/15.
//  Copyright (c) 2015 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_HUD_h
#define FreshApp_HUD_h

#include "Essentials.h"
#include "ProgrammingArea.h"

namespace ld
{
	
	class HUD : public fr::UIPopup
	{
		FRESH_DECLARE_CLASS( HUD, UIPopup );
	public:
		
		ProgrammingArea& programmingArea() const
		{
			return getExpectedDescendant< ProgrammingArea >( *this, "_programmingArea" );
		}
		
	private:
		
	};
	
}

#endif
