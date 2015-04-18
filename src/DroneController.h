//
//  DroneController.h
//  FreshApp
//
//  Created by Jeff Wofford on 4/17/15.
//  Copyright (c) 2015 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_DroneController_h
#define FreshApp_DroneController_h

#include "Essentials.h"
#include "FreshPlayerController.h"
#include "Drone.h"
#include "Program.h"

namespace ld
{
	
	class DroneController : public fr::FreshActorController
	{
		FRESH_DECLARE_CLASS( DroneController, FreshActorController );
	public:
		
		const vec2& dronePos() const { return drone().position(); }
		
		void loadProgramText( const std::string& programText );
		
		bool hasSensor( const std::string& sensorName ) const
		{
			return m_sensorValues.end() != m_sensorValues.find( sensorName );
		}
		
		bool hasControl( const std::string& controlName ) const
		{
			return m_controls.end() != m_controls.find( controlName );
		}
		
		const Program::Value& sensor( const std::string& sensorName ) const
		{
			auto iter = m_sensorValues.find( sensorName );
			if( iter != m_sensorValues.end() )
			{
				return iter->second;
			}
			else
			{
				throw createString( "Unrecognized sensor name \"" << sensorName << "\"." );
			}
		}

		void setControl( const std::string& controlName, const Program::Value& value )
		{
			auto iter = m_controls.find( controlName );
			if( iter != m_controls.end() )
			{
				iter->second.first = value;
			}
			else
			{
				throw createString( "Unrecognized control name \"" << controlName << "\"." );
			}
		}
		
		bool hasProgram() const
		{
			return !!m_program;
		}

		virtual void update() override;
		
		Drone& drone() const
		{
			return static_cast< Drone& >( *host() );
		}
		
	protected:

		void controlTargetPosition( const Program::Value& value );
		void controlDirection( const Program::Value& value );
		void controlFirePrimary( const Program::Value& value );
		void controlFireSecondary( const Program::Value& value );
		
	private:
		
		VAR( Program::ptr, m_program );
		
		typedef std::map< std::string, Program::Value > NamedValues;
		NamedValues m_sensorValues;

		typedef std::map< std::string, std::pair< Program::Value, std::function< void( const Program::Value& ) >>> Controls;
		Controls m_controls;
		Controls m_defaultControls;
	};
}

#endif
