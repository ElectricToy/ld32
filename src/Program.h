//
//  Program.h
//  FreshApp
//
//  Created by Jeff Wofford on 4/18/15.
//  Copyright (c) 2015 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_Program_h
#define FreshApp_Program_h

#include "Essentials.h"

namespace ld
{
	class DroneController;

	typedef std::vector< vec2 > array;

	template< typename ResultT, typename InitialT >
	ResultT convert( const InitialT& value )
	{
		return value;
	}
	
	template<>
	inline bool convert( const real& value ) { return value > 0; }
	template<>
	inline real convert( const vec2& value ) { return value.x; }
	template<>
	inline vec2 convert( const array& value ) { return value.empty() ? vec2( 0 ) : std::accumulate( value.begin(), value.end(), vec2( 0 ) ) / value.size(); }
	template<>
	inline real convert( const array& value ) { return convert< real >( convert< vec2 >( value )); }
	template<>
	inline bool convert( const vec2& value ) { return convert< bool >( convert< real >( value )); }
	template<>
	inline bool convert( const array& value ) { return convert< bool >( convert< real >( value )); }
	
	class Program : public fr::Object
	{
		FRESH_DECLARE_CLASS( Program, Object );
	public:

		struct Value
		{
			enum class Type
			{
				Undefined,
				Bool,
				Real,
				Vec2,
				Array,
			};
			
			Type type;
			
			union
			{
				bool b;
				real r;
				vec2 v;
				array a;
			};
			
			Value() : type( Type::Undefined ){}
			explicit Value( bool value ) : type( Type::Bool ), b( value ) {}
			explicit Value( real value ) : type( Type::Real ), r( value ) {}
			explicit Value( const vec2& value ) : type( Type::Vec2 ), v( value ) {}
			explicit Value( const array& value ) : type( Type::Array ), a( value ) {}
			explicit Value( array&& value ) : type( Type::Array ), a( std::move( value )) {}

			Value( const Value& other )
			{
				operator=( other );
			}
			
			~Value() {}
			
			Value& operator=( const Value& other )
			{
				if( this != &other )
				{
					type = other.type;
					switch( type )
					{
						case Type::Undefined:
							break;
						case Type::Bool:
							b = other.b;
							break;
						case Type::Real:
							r = other.r;
							break;
						case Type::Vec2:
							v = other.v;
							break;
						case Type::Array:
							a = other.a;
							break;
					}
				}
				
				return *this;
			}
			
			template< typename T >
			bool is() const;
			
			template< typename T >
			T get() const;
			
			template< typename T >
			void set( const T& value );
		};
		
		bool compiled() const { return !m_assignments.empty(); }
		void parse( const std::string& program );
		void execute( DroneController& controller );
		
		struct Expression
		{
			virtual ~Expression() {}
			
			virtual Value value( const DroneController& controller ) const = 0;
		};
		
		struct FunctionExpression : public Expression
		{
			typedef std::vector< std::shared_ptr< Program::Expression >> Arguments;
			
			std::string functionName;
			Arguments arguments;
			
			explicit FunctionExpression( const std::string& functionName_, Arguments&& arguments_ )
			:	functionName( functionName_ )
			,	arguments( std::move( arguments_ ))
			{}
			
			virtual Value value( const DroneController& controller ) const override;
		};
		
		struct SensorExpression : public Expression
		{
			std::string sensorName;
			
			explicit SensorExpression( const std::string& name_ ) : sensorName( name_ ) {}
			
			virtual Value value( const DroneController& controller ) const override;
		};
		
		struct Assignment
		{
			std::string controlName;
			std::shared_ptr< Expression > expression;
			
			explicit Assignment( const std::string& name_, const std::shared_ptr< Expression >& expression_ )
			:	controlName( name_ )
			,	expression( expression_ )
			{}
			
			virtual bool apply( DroneController& controller ) const;
		};
		
	private:
		
		std::vector< std::shared_ptr< Assignment >> m_assignments;
	};
	
	template< typename T >
	Program::Value::Type typeForType();
	
	template<>
	inline Program::Value::Type typeForType< bool >()
	{
		return Program::Value::Type::Bool;
	}
	
	template<>
	inline Program::Value::Type typeForType< real >()
	{
		return Program::Value::Type::Real;
	}
	
	template<>
	inline Program::Value::Type typeForType< vec2 >()
	{
		return Program::Value::Type::Vec2;
	}
	
	template<>
	inline Program::Value::Type typeForType< array >()
	{
		return Program::Value::Type::Array;
	}
	
	template< typename T >
	bool Program::Value::is() const
	{
		return type == typeForType< T >();
	}
	
	template<>
	inline bool Program::Value::get() const
	{
		switch( type )
		{
			case Type::Undefined:
				throw "Can't access undefined value.";
			case Type::Bool:
				return convert< bool >( b );
			case Type::Real:
				return convert< bool >( r );
			case Type::Vec2:
				return convert< bool >( v );
			case Type::Array:
				return convert< bool >( a );
		}
	}
	
	template<>
	inline real Program::Value::get() const
	{
		switch( type )
		{
			case Type::Undefined:
				throw "Can't access undefined value.";
			case Type::Bool:
				throw "Can't convert a bool to a real.";
			case Type::Real:
				return convert< real >( r );
			case Type::Vec2:
				return convert< real >( v );
			case Type::Array:
				return convert< real >( a );
		}
	}
	
	template<>
	inline vec2 Program::Value::get() const
	{
		switch( type )
		{
			case Type::Undefined:
				throw "Can't access undefined value.";
			case Type::Bool:
				throw "Can't convert a bool to a vec2.";
			case Type::Real:
				throw "Can't convert a real to a vec2.";
			case Type::Vec2:
				return convert< vec2 >( v );
			case Type::Array:
				return convert< vec2 >( a );
		}
	}
	
	template<>
	inline array Program::Value::get() const
	{
		switch( type )
		{
			case Type::Undefined:
				throw "Can't access undefined value.";
			case Type::Bool:
				throw "Can't convert a bool to an array.";
			case Type::Real:
				throw "Can't convert a real to an array.";
			case Type::Vec2:
				throw "Can't convert a vec2 to an array.";
			case Type::Array:
				return convert< array >( a );
		}
	}
	
	template<>
	inline void Program::Value::set( const bool& value )
	{
		b = value;
		type = Type::Bool;
	}
	
	template<>
	inline void Program::Value::set( const real& value )
	{
		r = value;
		type = Type::Real;
	}

	template<>
	inline void Program::Value::set( const vec2& value )
	{
		v = value;
		type = Type::Vec2;
	}

	template<>
	inline void Program::Value::set( const array& value )
	{
		a = value;
		type = Type::Array;
	}

}

#endif
