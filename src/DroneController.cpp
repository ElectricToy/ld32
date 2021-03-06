//
//  DroneController.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 4/17/15.
//  Copyright (c) 2015 Jeff Wofford. All rights reserved.
//

#include "DroneController.h"
#include "World.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( DroneController )
	DEFINE_VAR( DroneController, Program::ptr, m_program );
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTOR_INERT( DroneController )
	FRESH_CUSTOM_STANDARD_CONSTRUCTOR_NAMING( DroneController )
	{
		m_controls = m_defaultControls =
		{
			std::make_pair( "x", std::make_pair( Program::Value{ std::numeric_limits< real >::quiet_NaN() }, std::bind( &DroneController::controlTargetPosition, this, std::placeholders::_1 ))),
			std::make_pair( "dir", std::make_pair( Program::Value{ 0.0f }, std::bind( &DroneController::controlDirection, this, std::placeholders::_1 ))),
			std::make_pair( "fire", std::make_pair( Program::Value{ false }, std::bind( &DroneController::controlFirePrimary, this, std::placeholders::_1 ))),
			std::make_pair( "missile", std::make_pair( Program::Value{ false }, std::bind( &DroneController::controlFireSecondary, this, std::placeholders::_1 ))),
		};
	}

	void DroneController::loadProgramText( const std::string& programText )
	{
		m_program = createObject< Program >();
		m_program->parse( programText );
		m_controls = m_defaultControls;
	}
	
	void DroneController::update()
	{
		Super::update();
		
		// Load sensors.
		//
		m_sensorValues[ "x" ].set( drone().position().x );
		m_sensorValues[ "dir" ].set( sign( drone().effectiveVelocity().x ));
		m_sensorValues[ "time" ].set( static_cast< real >( drone().world().time() ));
		
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

	void DroneController::controlTargetPosition( const Program::Value& value )
	{
		const real targetX = value.get< real >();
		
		if( !isNaN( targetX ))
		{
			// Nullify direction. ('x' trumps 'dir')
			//
			m_controls[ "dir" ] = m_defaultControls[ "dir" ];
			
			real thrust = ( targetX - dronePos().x ) / 16.0f;
			
			thrust = clamp( thrust, -1.0f, 1.0f );
			
			drone().applyControllerImpulse( vec2( thrust, 0 ));
		}
	}
	
	void DroneController::controlDirection( const Program::Value& value )
	{
		real direction = value.get< real >();
		drone().applyControllerImpulse( vec2( sign( direction ), 0 ));
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

