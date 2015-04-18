//
//  DroneController.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 4/17/15.
//  Copyright (c) 2015 Jeff Wofford. All rights reserved.
//

#include "DroneController.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( DroneController )
	DEFINE_VAR( DroneController, Program::ptr, m_program );
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTOR_INERT( DroneController )
	FRESH_CUSTOM_STANDARD_CONSTRUCTOR_NAMING( DroneController )
	{
		m_controls =
		{
			std::make_pair( "direction", std::make_pair( Program::Value{ 0.0f }, std::bind( &DroneController::controlDirection, this, std::placeholders::_1 ))),
			std::make_pair( "fire", std::make_pair( Program::Value{ false }, std::bind( &DroneController::controlFirePrimary, this, std::placeholders::_1 ))),
			std::make_pair( "missile", std::make_pair( Program::Value{ false }, std::bind( &DroneController::controlFireSecondary, this, std::placeholders::_1 ))),
		};
	}

	void DroneController::loadProgramText( const std::string& programText )
	{
		m_program = createObject< Program >();
		m_program->parse( programText );
	}
	
	void DroneController::update()
	{
		Super::update();
		
		// Load sensors.
		//
		m_sensorValues[ "x" ].set( drone().position().x );
		
		// Execute the program.
		//
		if( m_program )
		{
			m_program->execute( *this );
		}
		
		// Apply controls.
		//
		for( const auto& control : m_controls )
		{
			const auto& value = control.second.first;
			const auto& fn = control.second.second;
			fn( value );
		}
	}

	void DroneController::controlDirection( const Program::Value& value )
	{
		real direction = value.get< real >();
		drone().applyControllerImpulse( vec2( direction, 0 ));
	}
	
	void DroneController::controlFirePrimary( const Program::Value& value )
	{
		drone().setTrigger( 0, value.get< bool >() );
	}
	
	void DroneController::controlFireSecondary( const Program::Value& value )
	{
		drone().setTrigger( 1, value.get< bool >() );
	}
}

