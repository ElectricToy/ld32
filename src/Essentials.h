//
//  Essentials.h
//  ld32
//
//  Created by Jeff Wofford on 2013/11/19.
//  Copyright (c) 2013 Jeff Wofford. All rights reserved.
//

#ifndef ld32_Essentials_h
#define ld32_Essentials_h

#include "FreshVector.h"
#include "Color.h"
#include "Object.h"
#include "PropertyTraits.h"
#include "ObjectPtr.h"
#include "CommandProcessor.h"
#include "MovieClip.h"
#include "ClassFilter.h"
#include "FreshFile.h"
#include "DisplayPackage.h"
#include "AudioSystem.h"
#include "StructSerialization.h"
#include "UIPopup.h"
#include "SimpleButton.h"
#include "Stage.h"
#include "FreshActor.h"
#include "FreshActorController.h"

#if FRESH_TELEMETRY_ENABLED
#	include "UserTelemetry.h"
#endif

namespace fr
{
	class Sound;
}

namespace ld
{

	using fr::ClassInfo;
	using fr::ClassName;
	using fr::path;
	using fr::real;
	using fr::uint;
	using fr::vec2;
	using fr::vec3;
	using fr::vec4;
	typedef fr::Vector2i vec2i;
	typedef fr::Vector2ui vec2ui;
	using fr::mat4;
	using fr::Color;
	using fr::SmartPtr;
	using fr::WeakPtr;
	using fr::TimeType;
	using fr::Range;
	using fr::rect;
	using fr::Rectanglei;
	using fr::angle;
	using fr::clamp;
	using fr::lerp;
	using fr::ObjectId;
	using fr::ClassFilter;
	using fr::dynamic_freshptr_cast;
	using fr::sign;
	
	template< typename T = fr::DisplayObject >
	T& getExpectedDescendant( const fr::DisplayObjectContainer& host, const std::string& nameSubstring = "" )
	{
		fr::SmartPtr< T > descendant = host.getDescendantByName< T >( nameSubstring, fr::DisplayObjectContainer::NameSearchPolicy::Substring );
		ASSERT( descendant );
		return *descendant;
	}
	
	const real WORLD_EXTENTS_X = 320.0f;
	const Range< real > WORLD_RANGE_X{ WORLD_EXTENTS_X * -0.5f, WORLD_EXTENTS_X * 0.5f };
}

#endif
