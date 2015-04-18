//
//  Program.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 4/18/15.
//  Copyright (c) 2015 Jeff Wofford. All rights reserved.
//

#include "Program.h"
#include "DroneController.h"
using namespace fr;

namespace
{
	using namespace ld;
	
	inline bool eof( int c )
	{
		return c == std::char_traits< char >::eof();
	}
	
	enum class TokenType
	{
		Identifier,
		ParenOpen,
		ParenClose,
		Assign,
		LessEqual,
		GreaterEqual,
		Less,
		Greater,
		And,
		Or,
		Not,
		EndOfFile,
		
		// Non-terminal tokens
		
		Expression,
	};
	
#define CASE( type ) case TokenType::type : out << #type; break;
	
	std::ostream& operator<<( std::ostream& out, TokenType type )
	{
		switch( type )
		{
			CASE( Identifier )
			CASE( ParenOpen )
			CASE( ParenClose )
			CASE( Assign )
			CASE( LessEqual )
			CASE( GreaterEqual )
			CASE( Less )
			CASE( Greater )
			CASE( And )
			CASE( Or )
			CASE( Not )
			CASE( EndOfFile )
			CASE( Expression )
			default: assert( false ); break;
		}
		
		return out;
	}
#undef CASE

	struct Token
	{
		TokenType type;
		std::string contents;
	};
	
#define CHECK_PENDING_IDENTIFIER	\
if( !contents.empty() )	\
{	\
	in.putback( c );	\
	return Token{ TokenType::Identifier, std::move( contents ) };	\
}

	std::unique_ptr< Token > g_peekedToken;

	Token nextToken( std::istream& in )
	{
		if( g_peekedToken )
		{
			Token result = *g_peekedToken;
			g_peekedToken.reset();
			return result;
		}
		
		std::string contents;
		
		while( in )
		{
			const char c = in.get();
			const char lookahead = in.peek();
			
			if( eof( c ))
			{
				CHECK_PENDING_IDENTIFIER
				
				assert( contents.empty() );
				break;
			}
			else if( c == '(' )
			{
				CHECK_PENDING_IDENTIFIER
				
				assert( contents.empty() );
				return Token{ TokenType::ParenOpen };
			}
			else if( c == ')' )
			{
				CHECK_PENDING_IDENTIFIER
				
				assert( contents.empty() );
				return Token{ TokenType::ParenClose };
			}
			else if( c == '=' )
			{
				CHECK_PENDING_IDENTIFIER
				
				assert( contents.empty() );
				return Token{ TokenType::Assign };
			}
			else if( c == '<' && lookahead == '=' )
			{
				CHECK_PENDING_IDENTIFIER
				
				assert( contents.empty() );
				return Token{ TokenType::LessEqual };
			}
			else if( c == '>' && lookahead == '=' )
			{
				CHECK_PENDING_IDENTIFIER
				
				assert( contents.empty() );
				return Token{ TokenType::GreaterEqual };
			}
			else if( c == '<' )
			{
				CHECK_PENDING_IDENTIFIER
				
				assert( contents.empty() );
				return Token{ TokenType::Less };
			}
			else if( c == '>' )
			{
				CHECK_PENDING_IDENTIFIER
				
				assert( contents.empty() );
				return Token{ TokenType::Greater };
			}
			else if( c == '|' )
			{
				CHECK_PENDING_IDENTIFIER
				
				if( lookahead != '|' )
				{
					throw;
				}
				
				assert( contents.empty() );
				return Token{ TokenType::Or };
			}
			else if( c == '&' )
			{
				CHECK_PENDING_IDENTIFIER
				
				if( lookahead != '&' )
				{
					throw;
				}
				
				assert( contents.empty() );
				return Token{ TokenType::And };
			}
			else if( c == '!' )
			{
				CHECK_PENDING_IDENTIFIER
				
				assert( contents.empty() );
				return Token{ TokenType::Not };
			}
			else if( std::isspace( c ))
			{
				// Got any content yet? If so, return it as an identifier or keyword. Else skip it.
				
				CHECK_PENDING_IDENTIFIER
				
				continue;
			}
			
			contents += c;
		}
		
		return Token{ TokenType::EndOfFile };
	}

	Token peekToken( std::istream& in )
	{
		if( g_peekedToken )
		{
			return *g_peekedToken;
		}
		else
		{
			auto token = nextToken( in );
			
			g_peekedToken.reset( new Token( token ) );
			return *g_peekedToken;
		}
	}

	Token requireToken( std::istream& in, TokenType type )
	{
		auto token = nextToken( in );
		
		if( token.type != type )
		{
			throw createString( "You had \"" << token.contents << "\" (" << token.type << ") where " << type << " was expected." );;
		}
		
		return token;
	}
	
	std::shared_ptr< Program::Expression > expression( std::istream& in );
	
	std::shared_ptr< Program::Expression > functionExpression( std::istream& in, Token functionNameToken )
	{
		Program::FunctionExpression::Arguments arguments;

		requireToken( in, TokenType::ParenOpen );
		
		while( peekToken( in ).type != TokenType::ParenClose )
		{
			arguments.push_back( std::move( expression( in )));
		}
		
		requireToken( in, TokenType::ParenClose );
		
		auto expression = std::make_shared< Program::FunctionExpression >( functionNameToken.contents, std::move( arguments ));
		return expression;
	}

	std::shared_ptr< Program::Expression > binaryOperatorExpression( std::istream& in, Token operatorToken )
	{
		Program::FunctionExpression::Arguments arguments;
		arguments.push_back( expression( in ));
		return std::make_shared< Program::FunctionExpression >( operatorToken.contents, std::move( arguments ));
	}

	std::shared_ptr< Program::Expression > infixExpression( std::istream& in, Token firstArgumentToken )
	{
		return nullptr;
		// TODO
	}
	
	std::shared_ptr< Program::Expression > expression( std::istream& in )
	{
		auto token = nextToken( in );
		
		switch( token.type )
		{
			case TokenType::ParenOpen:
			{
				auto expr = expression( in );
				requireToken( in, TokenType::ParenClose );
				return expr;
			}
			case TokenType::Identifier:
			{
				auto peeked = peekToken( in );
				
				switch( peeked.type )
				{
					case TokenType::ParenOpen:
						return functionExpression( in, token );
						
					case TokenType::LessEqual:
					case TokenType::GreaterEqual:
					case TokenType::Less:
					case TokenType::Greater:
					case TokenType::And:
					case TokenType::Or:
						return infixExpression( in, token );
						
					default:
						return std::make_shared< Program::SensorExpression >( token.contents );;
				}
			}
			case TokenType::Not:
				return binaryOperatorExpression( in, token );
				break;
				
			default:
				throw createString( "You had \"" << token.contents << "\" when it wasn't expected." );
		}
	}
	
	std::shared_ptr< Program::Assignment > assignment( std::istream& in )
	{
		auto control = requireToken( in, TokenType::Identifier );

		requireToken( in, TokenType::Assign );
		
		auto valueExpression = expression( in );
		return std::make_shared< Program::Assignment >( control.contents, valueExpression );
	}
	
	typedef std::map< std::string,
					  std::function< Program::Value( const DroneController&,
												     const Program::FunctionExpression::Arguments& ) >> FunctionMap;
	
	inline void requireArguments( const std::initializer_list< Program::Value::Type >& expectedArgumentTypes, const Program::FunctionExpression::Arguments& givenArguments )
	{
//		for( size_t i = 0; i < std::min( expectedArgumentTypes.size(), givenArguments.size() ); ++i )
//		{
//			if( expectedArgumentTypes[ i ] != givenArguments[ i ]->valueType )
//			{
//			}
//		}
		
		ptrdiff_t diff = givenArguments.size() - expectedArgumentTypes.size();
		
		if( diff != 0 )
		{
			throw createString( "Expected " << expectedArgumentTypes.size() << " arguments but you gave me " << givenArguments.size() );
		}
	}
	
	const FunctionMap FUNCTIONS =
	{
		std::make_pair( "!", []( const DroneController& controller, const Program::FunctionExpression::Arguments& arguments ) -> Program::Value
					   {
						   requireArguments( { Program::Value::Type::Bool }, arguments );
						   return Program::Value{ !( arguments[ 0 ]->value( controller ).get< bool >() ) };
					   } ),
		std::make_pair( "&&", []( const DroneController& controller, const Program::FunctionExpression::Arguments& arguments ) -> Program::Value
					   {
						   requireArguments( { Program::Value::Type::Bool, Program::Value::Type::Bool }, arguments );
						   return Program::Value{ ( arguments[ 0 ]->value( controller ).get< bool >() ) &&
												  ( arguments[ 1 ]->value( controller ).get< bool >() )};
					   } ),
		std::make_pair( "||", []( const DroneController& controller, const Program::FunctionExpression::Arguments& arguments ) -> Program::Value
					   {
						   requireArguments( { Program::Value::Type::Bool, Program::Value::Type::Bool }, arguments );
						   return Program::Value{ ( arguments[ 0 ]->value( controller ).get< bool >() ) ||
							   ( arguments[ 1 ]->value( controller ).get< bool >() )};
					   } ),
		std::make_pair( "<", []( const DroneController& controller, const Program::FunctionExpression::Arguments& arguments ) -> Program::Value
					   {
						   requireArguments( { Program::Value::Type::Real, Program::Value::Type::Real }, arguments );
						   return Program::Value{ ( arguments[ 0 ]->value( controller ).get< real >() ) <
											      ( arguments[ 1 ]->value( controller ).get< real >() )};
					   } ),
		std::make_pair( "<=", []( const DroneController& controller, const Program::FunctionExpression::Arguments& arguments ) -> Program::Value
					   {
						   requireArguments( { Program::Value::Type::Real, Program::Value::Type::Real }, arguments );
						   return Program::Value{ ( arguments[ 0 ]->value( controller ).get< real >() ) <=
							   ( arguments[ 1 ]->value( controller ).get< real >() )};
					   } ),
		std::make_pair( ">", []( const DroneController& controller, const Program::FunctionExpression::Arguments& arguments ) -> Program::Value
					   {
						   requireArguments( { Program::Value::Type::Real, Program::Value::Type::Real }, arguments );
						   return Program::Value{ ( arguments[ 0 ]->value( controller ).get< real >() ) >
							   ( arguments[ 1 ]->value( controller ).get< real >() )};
					   } ),
		std::make_pair( ">=", []( const DroneController& controller, const Program::FunctionExpression::Arguments& arguments ) -> Program::Value
					   {
						   requireArguments( { Program::Value::Type::Real, Program::Value::Type::Real }, arguments );
						   return Program::Value{ ( arguments[ 0 ]->value( controller ).get< real >() ) >=
							   ( arguments[ 1 ]->value( controller ).get< real >() )};
					   } ),
	};
}

namespace ld
{	
	FRESH_DEFINE_CLASS( Program )
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Program )
	
	void Program::parse( const std::string& program )
	{
		try
		{
			std::istringstream in( program );

			decltype( m_assignments ) newAssignments;
			
			while( in )
			{
				newAssignments.push_back( assignment( in ));
			}
			
			std::swap( m_assignments, newAssignments );
			
			release_trace( "Success. " << m_assignments.size() << " assignments." );
		}
		catch( const std::string& e )
		{
			release_error( e );
		}
	}
	
	void Program::execute( DroneController& controller )
	{
		for( auto iter = m_assignments.begin(); iter != m_assignments.end(); /* within */ )
		{
			const auto& assignment = *iter;
			bool succeeded = assignment->apply( controller );
			if( succeeded )
			{
				++iter;
			}
			else
			{
				iter = m_assignments.erase( iter );
			}
		}
	}

	Program::Value Program::FunctionExpression::value( const DroneController& controller ) const
	{
		auto iter = FUNCTIONS.find( functionName );
		if( iter != FUNCTIONS.end() )
		{
			try
			{
				return iter->second( controller, arguments );
			}
			catch( const std::string& e )
			{
				throw createString( "In function \"" << functionName << "\": " << e );
			}
		}
		else
		{
			throw createString( "Unrecognized function \"" << functionName << "\"." );
		}
	}
	
	Program::Value Program::SensorExpression::value( const DroneController& controller ) const
	{
		return controller.sensor( sensorName );
	}

	bool Program::Assignment::apply( DroneController& controller ) const
	{
		try
		{
			Value value = expression->value( controller );
			controller.setControl( controlName, value );
			return true;
		}
		catch( const std::string& e )
		{
			release_error( e );
			return false;
		}
	}
}

