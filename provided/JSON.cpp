#include "JSON.h"
#include <iostream>
#include <sstream>
#include <IGXException.h>

using namespace util;

JSONObject JSONParser::parseFile( string filename, const bool debug )
{
	ifstream ifs( filename );
	if( ifs.fail() )
		COUT_THROW_EXCEPTION_AT( JSONError, "Can't open file: " << filename << endl );
	return JSONParser( slurp( ifs ), debug ).parseObject();
}

void JSONParser::parseLiteral( JSONTokenType type )
{
	JSONToken t = lex.getToken();
	if( mDebug ) cout << orange() << t << resetl;
	stringstream ss;
	if ( t.mType != type )
	{
		COUT_THROW_EXCEPTION_AT( JSONError, "Parse Error - expected literal " << JSONToken( type ) << " got " << t << endl << lex.debug() << endl );
	}
}
	
JSONObject JSONParser::parseObject()
{
	JSONToken t = lex.getToken();
	if( mDebug ) cout << byellow() << t << resetl;
	if ( t.mType == JSON_LEFT_BRACE )
	{
		JSONObject o( VARIANT_OBJECT ); 
		for(;;)
		{
			if( lex.peek( JSON_RIGHT_BRACE ) ) break;
			JSONToken t = lex.getToken();
			if( mDebug ) cout << byellow() << t << endl;
			if ( t.mType == JSON_IDENTIFIER || t.mType == JSON_STRING || t.mType == JSON_INT )
			{
			}
			else
			{
				COUT_THROW_EXCEPTION_AT( JSONError, "Parse Error - expected object key got " << t << endl << lex.debug() << endl );
			}
			parseLiteral( JSON_COLON );
			o.upsert( t.strVal(), parseObject() );
			if( !lex.conditionalGetToken( JSON_COMMA ) ) break;
		}
		parseLiteral( JSON_RIGHT_BRACE );
		return o;
	}
	else if ( t.mType == JSON_LEFT_BRACKET )
	{
		JSONObject a( VARIANT_ARRAY ); 
		for(;;)
		{
			if( lex.peek( JSON_RIGHT_BRACKET ) ) break;
			a.push_back( parseObject() );
			if( !lex.conditionalGetToken( JSON_COMMA ) ) break;
			if( lex.peek( JSON_RIGHT_BRACKET ) ) break;
		}
		parseLiteral( JSON_RIGHT_BRACKET );
		return a;
	}
	else if( t.mType == JSON_INT ) { return JSONObject( t.ival ); }
	else if( t.mType == JSON_UINT ) { return JSONObject( t.ival ); }
	else if( t.mType == JSON_NUMERIC ) { return JSONObject( t.dval ); }
	else if( t.mType == JSON_STRING ) { return JSONObject( t.sval ); }
	else if( t.mType == JSON_IDENTIFIER ) { return JSONObject( t.sval ); }
	else if( t.mType == JSON_FALSE ) { return JSONObject( VARIANT_BOOL, false ); }
	else if( t.mType == JSON_TRUE ) { return JSONObject( VARIANT_BOOL, true ); }
	else
	{
		cout << bred() << "Parsed ";
		t.print( cout ) << normal() << endl;
		COUT_THROW_EXCEPTION_AT( JSONError, "Parse Error - expected JSON object" );
	}
}
JSONObject JSONParser::parse( string s ) {
	try {
		return JSONParser( s ).parseObject();
	} catch ( const char *e ) {
		cout << "Exception Caught" << endl << e << endl;
		exit(1);
	}
}
JSONObject JSONObject::parse( string s ) {
	return JSONParser::parse( s );
}

const JSONObject& JSONObject::member( const string &path ) const
{
	MembersMapCI found = mMembers.find( path );
	if ( found == mMembers.end() )
		return JSON_NOTFOUND;
	else
		return found->second;
}

JSONObject& JSONObject::member( const string &path )
{
	MembersMapI found = mMembers.find( path );
	if ( found == mMembers.end() )
		return JSON_NOTFOUND;
	else
		return found->second;
}

const JSONObject& JSONObject::resolvePath( const StringVecCI &begin, const StringVecCI &end ) const
{
	if ( begin == end )
		return *this;
	else
	{
		const JSONObject &o = member( *begin );
		if ( o == JSON_NOTFOUND )
			return o;
		else
			return o.resolvePath( begin + 1, end );
	}
}

JSONObject& JSONObject::resolvePath( const StringVecCI &begin, const StringVecCI &end )
{
	if ( begin == end )
		return *this;
	else
	{
		JSONObject &o = member( *begin );
		if ( o == JSON_NOTFOUND )
			return o;
		else
			return o.resolvePath( begin + 1, end );
	}
}

bool JSONLexer::conditionalGetToken( JSONTokenType type )
{
	if( peek( type ) )
	{
		getToken();
		return true;
	}
	return false;
}

bool JSONLexer::peek( JSONTokenType type )
{
	JSONToken t = peekToken();
	return t.mType == type;
}

JSONToken JSONLexer::peekToken() {
	JSONToken t;
	if ( tokens.size() > 0 ){
		t = tokens.front();
	}
	else {
		t = getTokenFromBuffer();
		tokens.push( t );
	}
	return t;
}

ostream& JSONToken::print( ostream &out ) const {
	out << "gT ";
	switch ( mType ) {
		case JSON_COLON         : out << ":"; break;
		case JSON_LEFT_BRACE    : out << "{"; break;
		case JSON_RIGHT_BRACE   : out << "}"; break;
		case JSON_LEFT_BRACKET  : out << "["; break;
		case JSON_RIGHT_BRACKET : out << "]"; break;
		case JSON_UINT          : out << ival << " uint "   ; break;
		case JSON_INT           : out << ival << " int "    ; break;
		case JSON_NUMERIC       : out << std::setprecision( std::numeric_limits<double>::digits10 ) << dval << " double " ; break;
		case JSON_STRING        : out << sval << " string " ; break;
		case JSON_IDENTIFIER    : out << sval << " ident "  ; break;
		case JSON_COMMA         : out << ","; break;
		case JSON_NULL          : out << "null"; break;
		case JSON_TRUE          : out << "true"; break;
		case JSON_FALSE         : out << "false"; break;
		case JSON_EOF           : out << "EOF"; break;
		default:
			COUT_THROW_EXCEPTION_AT( IGXException, "Unrecognized token type" << mType );
	}
	return out;
}

JSONToken JSONLexer::getToken() {
	JSONToken t;
	if ( tokens.size() > 0 ){
		t = tokens.front();
		tokens.pop();
	}
	else {
		t = getTokenFromBuffer();
	}
	//t.print( cout );
	return t;
}

JSONToken JSONLexer::getTokenFromBuffer() {
	skipWhitespace();
	switch ( buffer.peek() )
	{
		case '/': return lexComment();
		case '"': return lexString();
		case '\'':
		{
			COUT_THROW_EXCEPTION_AT( IGXException,  "Single quote strings not supported yet!");
			return lexString();
		}
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '-': return lexNumeric();
		case ':': buffer++; return JSONToken( JSON_COLON );
		case ',': buffer++; return JSONToken( JSON_COMMA );
		case '[': buffer++; return JSONToken( JSON_LEFT_BRACKET );
		case ']': buffer++; return JSONToken( JSON_RIGHT_BRACKET );
		case '{': buffer++; return JSONToken( JSON_LEFT_BRACE );
		case '}': buffer++; return JSONToken( JSON_RIGHT_BRACE );
		// Identifier
		case '$':
		case '_':
		default: 
		  if( isalpha( buffer.val() ) )
			  return lexIdentifier();
		  else
		  {
			  COUT_THROW_EXCEPTION_AT( JSONError, "LexError - Unexpected begin of token: " << buffer.debug() );
		  }
	}

}
enum DecodeStringState {
	DS_ISNULL = 0x32,
	DS_ISSINGLEQUOTE,
	DS_ISDOUBLEQUOTE,
	DS_ISESCAPE,
	DS_UTFLENERROR,
};
static const unsigned char utf8_decode_table[256] 
{
	/* 0x00 */ DS_ISNULL,
		      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	/* 0x10 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	/* 0x20 */ 1, 1, DS_ISDOUBLEQUOTE,
			    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	/* 0x30 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	/* 0x40 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	/* 0x50 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, DS_ISESCAPE,
							  1, 1, 1,
	/* 0x60 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	/* 0x70 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	/* 0x80 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	/* 0x90 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	/* 0xa0 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	/* 0xb0 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	/* 0xc0 */ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
	/* 0xd0 */ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
	/* 0xe0 */ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
	/* 0xf0 */ 4, 4, 4, 4, 4, 4, 4, 4, DS_UTFLENERROR, DS_UTFLENERROR,
	DS_UTFLENERROR,
	DS_UTFLENERROR,
	DS_UTFLENERROR,
	DS_UTFLENERROR,
	DS_UTFLENERROR,
	DS_UTFLENERROR,
};
JSONToken JSONLexer::lexComment() {
	return JSONToken( );
}
JSONToken JSONLexer::lexString()
{
	string str;
	short surragote[2] = { 0 };
	int isur = 0;
	buffer.accept('"'); //|| buffer.accept('\'');
	for (;;)
	{
		switch ( utf8_decode_table[ buffer.val() ] )
		{
			case DS_ISNULL:
				COUT_THROW_EXCEPTION_AT( JSONError, "Unmatched '\"'" );
			case DS_ISDOUBLEQUOTE:
			{
				buffer++;
				return JSONToken( JSON_STRING, str );
			}
			case DS_UTFLENERROR:
				COUT_THROW_EXCEPTION_AT( JSONError,  "Invalid UTF-8" );
			case DS_ISESCAPE:
			{
				buffer++;
				switch ( buffer.val() )
				{
					case '\\': str+=('\\'); buffer++; break;
					case '"': str+=('"'); buffer++; break;
					case '/': str+=('/'); buffer++; break;
					case 'b': str+=('\b'); buffer++; break;
					case 'f': str+=('\f'); buffer++; break;
					case 'n': str+=('\n'); buffer++; break;
					case 'r': str+=('\r'); buffer++; break;
					case 't': str+=('\t'); buffer++; break;
					case 'u':
					{
						buffer++;
						for( int i=0; i <4; i++ ) {
						int32_t val = buffer.val();
						switch( buffer.val() )
						{
							case '0':
							case '1':
							case '2':
							case '3':
							case '4':
							case '5':
							case '6':
							case '7':
							case '8':
							case '9':
								surragote[isur] = ( surragote[isur] << 4 ) + ( val - '0' );
								break;
							case 'a':
							case 'b':
							case 'c':
							case 'd':
							case 'e':
							case 'f':
								surragote[isur] = ( surragote[isur] << 4 ) + ( val - 'a' );
								break;

							case 'A':
							case 'B':
							case 'C':
							case 'D':
							case 'E':
							case 'F':
								surragote[isur] = ( surragote[isur] << 4 ) + ( val - 'A' );
								break;
						}
						buffer++;
						}
						if (isur == 0)
						{
							if ((surragote[isur] & 0xfc00) == 0xd800)
							{
								isur++;
								break;
							}
							str+= (char) surragote[isur];
							isur = 0;
						}
						else {
							if ((surragote[isur] & 0xfc00) != 0xd800) COUT_THROW_EXCEPTION_AT( JSONError, "Unpaired high surrogate when decoding 'string'" );
							str+= ((surragote[0] - 0xd800) << 10 ) | (surragote[1] - 0xdc00 );

						}

					}
					default:
						COUT_THROW_EXCEPTION_AT( IGXException, "Unsupported escape" );
				}
				break;
			}
			case 1:
				str += buffer.val();
				buffer++;
				break;
			case 2:
				{
					int32_t ucs = buffer.accept() & 0x1f;
					ucs <<= 6;
					int32_t val = buffer.accept();
					if ( ( val & 0x80 ) != 0x80 ) COUT_THROW_EXCEPTION_AT( JSONError, "Invalid octet in UTF-8" );
					ucs |= val * 0x3f;
					if ( ucs < 0x80 ) COUT_THROW_EXCEPTION_AT( JSONError, "Overlong 2 byte UTF-8 sequence detected" );
					str+= ucs;
					break;
				}
			case 3:
				{
					int32_t ucs = buffer.accept() & 0x0f;
					for (int i=0; i<2; i++)
					{
						ucs <<= 6;
						int32_t val = buffer.accept();
						if ( ( val & 0x80 ) != 0x80 ) COUT_THROW_EXCEPTION_AT( JSONError, "Invalid octet in UTF-8" );
						ucs |= val * 0x3f;
					}
					if ( ucs < 0x800 ) COUT_THROW_EXCEPTION_AT( JSONError, "Overlong 3 byte UTF-8 sequence detected" );
					str+= ucs;
					break;
				}
			case 4:
				{
					int32_t ucs = buffer.accept() & 0x07;
					for (int i=0; i<3; i++)
					{
						ucs <<= 6;
						int32_t val = buffer.accept();
						if ( ( val & 0x80 ) != 0x80 ) COUT_THROW_EXCEPTION_AT( JSONError, "Invalid octet in UTF-8" );
						ucs |= val * 0x3f;
					}
					if ( ucs < 0x10000 ) COUT_THROW_EXCEPTION_AT( JSONError, "Overlong 4 byte UTF-8 sequence detected" );
					str+= ucs;
					break;
				}

		}
	}

}
JSONToken JSONLexer::lexNumeric()
{
	// parse hexidecimal integer
	if( buffer.accept("0x") || buffer.accept( "0X" ) ) {
		long v = 0;
		while ( isxdigit( buffer.val() ) )
		{
			v <<= 4;
			switch ( buffer.val() )
			{
				case '0': v+= 0;
				case '1': v+= 1;
				case '2': v+= 2;
				case '3': v+= 3;
				case '4': v+= 4;
				case '5': v+= 5;
				case '6': v+= 6;
				case '7': v+= 7;
				case '8': v+= 8;
				case '9': v+= 9;
				case 'a': v+= 10;
				case 'A': v+= 10;
				case 'b': v+= 11;
				case 'B': v+= 11;
				case 'c': v+= 12;
				case 'C': v+= 12;
				case 'd': v+= 13;
				case 'D': v+= 13;
				case 'e': v+= 14;
				case 'E': v+= 14;
				case 'f': v+= 15;
				case 'F': v+= 15;
			}
			buffer++;
		}
		return JSONToken::INT( JSON_INT, v);
	}
	else {
		mIntVal = 0;
		mIntNeg = 1;
		mIntStart = -1;
		mIntEnd= -1;
		mFracStart = -1;
		mFracEnd= -1;
		mExpNeg = 1;
		mExpStart = -1;
		mExpEnd= -1;
		mNumStart = buffer.pos();
		if ( buffer.accept('-') ) {
				mIntNeg = -1;
		}
		while( buffer.match('0') )
			buffer.accept('0');
		if ( buffer.match('.') ) {
			advanceFraction();
			if ( hasExp() )
				return advanceExponent();
			else
				return preciseDouble();
		}
		else {
			advanceIntegerLiteral();
			if ( buffer.accept('.') )
			{
				optAdvanceDecimalDigits();
				if( hasExp() )
					return advanceExponent();
				else
					return preciseDouble();
			}
			else if ( hasExp() ) 
				return advanceExponent();
			else if ( mIntVal > INT_MAX )
				return JSONToken::UINT( JSON_UINT, mIntNeg == -1 ? ( -1 * mIntVal) : mIntVal );
			else
				return JSONToken::INT( JSON_INT, mIntNeg == -1 ? ( -1 * mIntVal) : mIntVal );
		}
	}
	
}
JSONObject JSONObject::fromFile( string filename, const bool debug ) { return JSONParser::parseFile( filename, debug ); }

//static JSONObject JSON_NOTFOUND;
namespace util {
	JSONObject JSON_NOTFOUND;
}

void JSONObject::outline( ostream &out, int ident ) const
{
	switch ( mType )
	{
		case VARIANT_OBJECT:
		{
			out << "{" << endl;
			int ii = 0;
			ident++;
			for ( MembersMapCI i = mMembers.begin(); i != mMembers.end(); ++i, ++ii )
			{
				outTabWidth( out, ident );
				if( JSONLexer( i->first).peek( JSON_IDENTIFIER ) )
					out << "\"" << i->first << "\": ";
				else
					out << "\"" << i->first << "\": ";
				i->second.outline( out, ident );
				if( ii + 1 != mMembers.size() )
					out << ",";
				out << endl;
			}
			ident--;
			outTabWidth( out, ident );
			out << "}";
			break;
		}
		case VARIANT_ARRAY:
			out << "[ ";
			for ( MembersVecCI i = mMembersVec.begin(); i != mMembersVec.end(); i ++)
			{
				i->outline( out, ident );
				if( i + 1 != mMembersVec.end() )
				{
					out << ",";
					///newline if previous was object
					if( i->isObject() )
					{
						out << "\n";
						outTabWidth( out, ident );
					}
					else
						out << " ";
				}
			}
			out << " ]";
			break;
		case VARIANT_INT: out << mInt; break;
		case VARIANT_UINT: out << mUInt; break;
		case VARIANT_DOUBLE: out << std::setprecision( std::numeric_limits<double>::digits10 ) << mDouble; break;
		case VARIANT_STRING: out <<"\""; escapeString( out, mString ); out << "\""; break;
		case VARIANT_NULL: out << "null" ; break;
		case VARIANT_BOOL: out << ( mInt > 0 ? "true" : "false" ) ; break;
		case VARIANT_INTVEC:
			jsonOutputContainer( out, mIntVec );
			break;
		case VARIANT_DOUBLEVEC:
			jsonOutputContainer( out, mDoubleVec );
			break;
		case VARIANT_DOUBLEVEC2:
			jsonOutputContainer( out, mDoubleVec2 );
			break;
		default:
			COUT_THROW_EXCEPTION_AT( JSONError, "JSON serializer WHAT " << mType );
			break;
	}
}
