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
		Null,
		Number,
		Identifier,
		ParenOpen,
		ParenClose,
		Assign,
		Terminator,
		Separator,
		BoolInfixOperator,
		ArithmeticComparatorOperator,
		LowArithmeticInfixOperator,
		HighArithmeticInfixOperator,
		PrefixOperator,
		Minus,
		EndOfFile,
		
		// Non-terminal tokens
		
		Term,
		Expression,
	};
	
#define CASE( type ) case TokenType::type : out << #type; break;
	
	std::ostream& operator<<( std::ostream& out, TokenType type )
	{
		switch( type )
		{
			CASE( Null )
			CASE( Number )
			CASE( Identifier )
			CASE( ParenOpen )
			CASE( ParenClose )
			CASE( Assign )
			CASE( Terminator )
			CASE( Separator )
			CASE( Minus )
			CASE( BoolInfixOperator )
			CASE( ArithmeticComparatorOperator )
			CASE( LowArithmeticInfixOperator )
			CASE( HighArithmeticInfixOperator )
			CASE( PrefixOperator )
			CASE( EndOfFile )
			CASE( Term )
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
		std::shared_ptr< Program::Expression > expression;
	};
	
	inline Token numberOrIdentifier( std::string&& contents )
	{
		std::istringstream destringifier( contents );
		real number;
		destringifier >> number;
		
		TokenType type = !destringifier.fail() ? TokenType::Number : TokenType::Identifier;
		return Token{ type, std::move( contents )};
	}
	
#define CHECK_PENDING_IDENTIFIER	\
if( !contents.empty() )	\
{	\
	in.putback( c );	\
	return numberOrIdentifier( std::move( contents ));	\
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
				contents += c;
				return Token{ TokenType::ParenOpen, contents };
			}
			else if( c == ')' )
			{
				CHECK_PENDING_IDENTIFIER
				
				assert( contents.empty() );
				contents += c;
				return Token{ TokenType::ParenClose, contents };
			}
			else if( c == '=' )
			{
				CHECK_PENDING_IDENTIFIER
				
				assert( contents.empty() );
				contents += c;
				return Token{ TokenType::Assign, contents };
			}
			else if( c == ';' )
			{
				CHECK_PENDING_IDENTIFIER
				
				assert( contents.empty() );
				contents += c;
				return Token{ TokenType::Terminator, contents };
			}
			else if( c == ',' )
			{
				CHECK_PENDING_IDENTIFIER
				
				assert( contents.empty() );
				contents += c;
				return Token{ TokenType::Separator, contents };
			}
			else if(( c == '<' || c == '>' ) && lookahead == '=' )
			{
				CHECK_PENDING_IDENTIFIER
				
				assert( contents.empty() );
				contents += c;
				contents += lookahead;
				in.get();
				return Token{ TokenType::ArithmeticComparatorOperator, contents };
			}
			else if( c == '<' || c == '>' )
			{
				CHECK_PENDING_IDENTIFIER
				
				assert( contents.empty() );
				contents += c;
				return Token{ TokenType::ArithmeticComparatorOperator, contents };
			}
			else if( c == '+' )
			{
				CHECK_PENDING_IDENTIFIER
				
				assert( contents.empty() );
				contents += c;
				return Token{ TokenType::LowArithmeticInfixOperator, contents };
			}
			else if( c == '*' || c == '/' )
			{
				CHECK_PENDING_IDENTIFIER
				
				assert( contents.empty() );
				contents += c;
				return Token{ TokenType::HighArithmeticInfixOperator, contents };
			}
			else if(( c == '|' || c == '&' ) && c == lookahead )
			{
				CHECK_PENDING_IDENTIFIER
				
				assert( contents.empty() );
				contents += c;
				contents += lookahead;
				in.get();
				return Token{ TokenType::BoolInfixOperator, contents };
			}
			else if( c == '!' )
			{
				CHECK_PENDING_IDENTIFIER
				
				assert( contents.empty() );
				contents += c;
				return Token{ TokenType::PrefixOperator, contents };
			}
			else if( c == '-' )
			{
				CHECK_PENDING_IDENTIFIER
				
				assert( contents.empty() );
				contents += c;
				return Token{ TokenType::Minus, contents };
			}
			else if( std::isspace( c ))
			{
				// Got any content yet? If so, return it as an identifier or keyword. Else skip it.
				
				CHECK_PENDING_IDENTIFIER
				
				continue;
			}
			
			contents += c;
		}
		
		if( contents.empty() == false )
		{
			return numberOrIdentifier( std::move( contents ));
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

	std::ostream& operator<<( std::ostream& out, const std::initializer_list< TokenType >& types )
	{
		bool first = true;
		for( const auto& type : types )
		{
			if( !first ) out << ", ";
			out << type;
			first = false;
		}
		
		return out;
	}

	Token requireToken( std::istream& in, const std::initializer_list< TokenType >& types )
	{
		auto token = nextToken( in );
		
		if( std::find( types.begin(), types.end(), token.type ) == types.end() )
		{
			throw createString( "You had \"" << token.contents << "\" (" << token.type << ") where " << types << " was expected." );;
		}
		
		return token;
	}

	Token requireToken( std::istream& in, TokenType type )
	{
		return requireToken( in, { type } );
	}
	
	Token expression( std::istream& in );

	Token functionExpression( std::istream& in, Token functionNameToken )
	{
		Program::FunctionExpression::Arguments arguments;

		requireToken( in, TokenType::ParenOpen );
		
		while( peekToken( in ).type != TokenType::ParenClose )
		{
			arguments.push_back( std::move( expression( in ).expression ));
			
			if( peekToken( in ).type == TokenType::Separator )
			{
				nextToken( in );
			}
		}
		
		requireToken( in, TokenType::ParenClose );
		
		auto expression = std::make_shared< Program::FunctionExpression >( functionNameToken.contents, std::move( arguments ));
		return Token{ TokenType::Expression, "", expression };
	}

	Token unaryOperatorExpression( std::istream& in, Token operatorToken )
	{
		Program::FunctionExpression::Arguments arguments;
		arguments.push_back( expression( in ).expression );
		auto expression = std::make_shared< Program::FunctionExpression >( operatorToken.contents, std::move( arguments ));
		return Token{ TokenType::Expression, "", expression };
	}

	Token infixExpression( std::istream& in, Token firstArgumentToken )
	{
		Program::FunctionExpression::Arguments arguments;
		
		arguments.push_back( firstArgumentToken.expression );
		
		auto operatorToken = requireToken( in, {
			TokenType::LowArithmeticInfixOperator,
			TokenType::Minus,
			TokenType::HighArithmeticInfixOperator,
			TokenType::ArithmeticComparatorOperator,
			TokenType::BoolInfixOperator,
		});
		
		auto secondArgument = expression( in ).expression;
		ASSERT( secondArgument );
		arguments.push_back( secondArgument );
		
		auto expression = std::make_shared< Program::FunctionExpression >( operatorToken.contents, std::move( arguments ));
		return Token{ TokenType::Expression, "", expression };
	}

	
	Token expression( std::istream& in )
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
						
					case TokenType::LowArithmeticInfixOperator:
					case TokenType::Minus:
					case TokenType::HighArithmeticInfixOperator:		// TODO
					case TokenType::ArithmeticComparatorOperator:		// TODO
					case TokenType::BoolInfixOperator:					// TODO
						return infixExpression( in, Token{ TokenType::Expression, "", std::make_shared< Program::SensorExpression >( token.contents ) } );
						
					default:
						return Token{ TokenType::Expression, "", std::make_shared< Program::SensorExpression >( token.contents ) };
				}
			}
			case TokenType::Number:
			{
				std::istringstream destringifier( token.contents );
				real number;
				destringifier >> number;
				token = Token{ TokenType::Expression, token.contents, std::make_shared< Program::LiteralExpression >( Program::Value{ number }) };
				auto peeked = peekToken( in );
				switch( peeked.type )
				{
					case TokenType::LowArithmeticInfixOperator:
					case TokenType::Minus:
					case TokenType::HighArithmeticInfixOperator:		// TODO
					case TokenType::ArithmeticComparatorOperator:		// TODO
					case TokenType::BoolInfixOperator:					// TODO
						return infixExpression( in, token );
						
					default:
						return token;
				}
			}
			case TokenType::Minus:
			case TokenType::PrefixOperator:
				return unaryOperatorExpression( in, token );

			default:
				return token;
		}
	}
	
	std::shared_ptr< Program::Assignment > assignment( std::istream& in )
	{
		if( peekToken( in ).type == TokenType::EndOfFile )
		{
			return nullptr;
		}
		
		auto control = requireToken( in, TokenType::Identifier );

		requireToken( in, TokenType::Assign );

		Token lastToken{ TokenType::Null };
		
		auto valueExpression = expression( in );
		ASSERT( valueExpression.type == TokenType::Expression );
		ASSERT( valueExpression.expression );
		return std::make_shared< Program::Assignment >( control.contents, valueExpression.expression );
	}
	
	typedef std::map< std::string,
					  std::function< Program::Value( const DroneController&,
												     const Program::FunctionExpression::Arguments& ) >> FunctionMap;
	
	inline void requireArguments( const std::initializer_list< Program::Value::Type >& expectedArgumentTypes, const Program::FunctionExpression::Arguments& givenArguments )
	{
//		for( size_t i = 0; i < std::min( expectedArgumentTypes.size(), givenArguments.size() ); ++i )
//		{
//			if( expectedArgumentTypes[ i ] != Program::Value::Type::Undefined && expectedArgumentTypes[ i ] != givenArguments[ i ]->valueType )
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
		std::make_pair( "+", []( const DroneController& controller, const Program::FunctionExpression::Arguments& arguments ) -> Program::Value
					   {
						   requireArguments( { Program::Value::Type::Real, Program::Value::Type::Real }, arguments );
						   return Program::Value{ ( arguments[ 0 ]->value( controller ).get< real >() ) +
							   ( arguments[ 1 ]->value( controller ).get< real >() )};
					   } ),
		std::make_pair( "-", []( const DroneController& controller, const Program::FunctionExpression::Arguments& arguments ) -> Program::Value
					   {
						   // TODO vec2.
						   if( arguments.size() == 1 )
						   {
							   // Unary
							   return Program::Value{ -( arguments[ 0 ]->value( controller ).get< real >() ) };
						   }
						   else if( arguments.size() == 2 )
						   {
							   // Binary
							   return Program::Value{ ( arguments[ 0 ]->value( controller ).get< real >() ) -
								   ( arguments[ 1 ]->value( controller ).get< real >() )};
						   }
						   else
						   {
							   requireArguments( { Program::Value::Type::Real }, arguments );
							   return Program::Value{ 0.0f };
						   }
					   } ),
		std::make_pair( "*", []( const DroneController& controller, const Program::FunctionExpression::Arguments& arguments ) -> Program::Value
					   {
						   requireArguments( { Program::Value::Type::Real, Program::Value::Type::Real }, arguments );
						   return Program::Value{ ( arguments[ 0 ]->value( controller ).get< real >() ) *
							   ( arguments[ 1 ]->value( controller ).get< real >() )};
					   } ),
		std::make_pair( "/", []( const DroneController& controller, const Program::FunctionExpression::Arguments& arguments ) -> Program::Value
					   {
						   requireArguments( { Program::Value::Type::Real, Program::Value::Type::Real }, arguments );
						   return Program::Value{ ( arguments[ 0 ]->value( controller ).get< real >() ) *
							   ( arguments[ 1 ]->value( controller ).get< real >() )};
					   } ),
		std::make_pair( "sin", []( const DroneController& controller, const Program::FunctionExpression::Arguments& arguments ) -> Program::Value
					   {
						   requireArguments( { Program::Value::Type::Real }, arguments );
						   return Program::Value{ std::sin( arguments[ 0 ]->value( controller ).get< real >()) };
					   } ),
		std::make_pair( "cos", []( const DroneController& controller, const Program::FunctionExpression::Arguments& arguments ) -> Program::Value
					   {
						   requireArguments( { Program::Value::Type::Real }, arguments );
						   return Program::Value{ std::cos( arguments[ 0 ]->value( controller ).get< real >()) };
					   } ),
		std::make_pair( "if", []( const DroneController& controller, const Program::FunctionExpression::Arguments& arguments ) -> Program::Value
					   {
						   requireArguments( { Program::Value::Type::Bool, Program::Value::Type::Undefined, Program::Value::Type::Undefined }, arguments );
						   return Program::Value{
							   ( arguments[ 0 ]->value( controller ).get< bool >() ) ?
							     arguments[ 1 ]->value( controller ) :
							     arguments[ 2 ]->value( controller )};
					   } ),
		std::make_pair( "awayfrom", []( const DroneController& controller, const Program::FunctionExpression::Arguments& arguments ) -> Program::Value
					   {
						   requireArguments( { Program::Value::Type::Real }, arguments );
						   return Program::Value{ controller.dronePos().x - arguments[ 0 ]->value( controller ).get< real >() };
					   } ),
		std::make_pair( "toward", []( const DroneController& controller, const Program::FunctionExpression::Arguments& arguments ) -> Program::Value
					   {
						   requireArguments( { Program::Value::Type::Real }, arguments );
						   return Program::Value{ arguments[ 0 ]->value( controller ).get< real >() - controller.dronePos().x };
					   } ),
	};
}

namespace ld
{	
	FRESH_DEFINE_CLASS( Program )
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Program )
	
	void Program::parse( const std::string& program )
	{
		g_peekedToken.reset();
		try
		{
			std::istringstream in( program );

			decltype( m_assignments ) newAssignments;
			
			while( in )
			{
				if( auto assign = assignment( in ))
				{
					newAssignments.push_back( assign );
				}
				else
				{
					break;
				}
			}
			
			std::swap( m_assignments, newAssignments );
			
			release_trace( "Success. " << m_assignments.size() << " assignments." );
		}
		catch( const std::string& e )
		{
			release_error( e );
		}
		g_peekedToken.reset();
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

