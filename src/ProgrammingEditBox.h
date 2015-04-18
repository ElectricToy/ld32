//
//  ProgrammingEditBox.h
//  FreshApp
//
//  Created by Jeff Wofford on 4/18/15.
//  Copyright (c) 2015 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_ProgrammingEditBox_h
#define FreshApp_ProgrammingEditBox_h

#include "Essentials.h"
#include "UIEditBox.h"

namespace ld
{
	
	class ProgrammingEditBox : public fr::UIEditBox
	{
		FRESH_DECLARE_CLASS( ProgrammingEditBox, UIEditBox );
	public:
		
		// TODO
		
	private:
		
		FRESH_DECLARE_CALLBACK( ProgrammingEditBox, onKeyDown, fr::EventKeyboard )
	};
	
}

#endif
