#ifndef JSON_H
#define JSON_H

#include "common_util.h"
#include <queue>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <initializer_list>

namespace util {

	typedef StringVec::const_iterator StringVecCI;

	inline StringVec &split(const string &s, char delim, StringVec &elems) {
	    stringstream ss(s);
	    string item;
	    while (std::getline(ss, item, delim)) {
		    elems.push_back(item);
	    }
	    return elems;
	}


	inline StringVec split(const string &s, char delim) {
	    StringVec elems;
	    split(s, delim, elems);
	    return elems;
	}

	inline ostream &escapeString( ostream &out, string s ){
		for( int i =0; i < s.size(); i++ )
		{
			switch (s[i])
			{
				case '\n': out << "\n"; break;
				case '\r': out << "\r"; break;
				case '\f': out << "\f"; break;
				case '\t': out << "\t"; break;
				case '\"': out << "\""; break;
				case '\\': out << "\\\\"; break;
				default:
					if ( s[i] < 32 || s[i] == 127 )
						out << "\\a" << setw(4) << setfill('0') << static_cast<int>(s[i]);
					else
						out << s[i]; break;
			}
		}
		return out;
	}

	enum JSONTokenType {
		JSON_COLON,
		JSON_LEFT_BRACE,
		JSON_RIGHT_BRACE,
		JSON_LEFT_BRACKET,
		JSON_RIGHT_BRACKET,
		JSON_UINT,
		JSON_INT,
		JSON_NUMERIC,
		JSON_STRING,
		JSON_IDENTIFIER,
		JSON_COMMA,
		JSON_NULL,
		JSON_TRUE,
		JSON_FALSE,
		JSON_EOF,
	};

	class Buffer { 
		public:
		Buffer() : mPos( 0 ), mCol( 1 ), mLines( 1 ) {}
		Buffer( string b ) : Buffer()
		{
			mB = b;
		}

		int peek() { 
			if( mPos == mB.length() )
				return EOF;
			return mB[mPos];
		}
		int operator++(int)
		{
			if( mB[mPos] == '\n' ){
				++mLines;
				mCol = 1;
			}
			++mPos;
			++mCol;
			return mPos;
		}
		int pos() const { return mPos; }
		int val() { return peek(); }
		string substr(int pos1, int pos2) {
			return mB.substr( pos1, pos2 );
		}

		int accept() { return mB[ operator++( 0 ) ]; }
		bool accept( const string buf )
		{
			if( ( mPos + buf.length() <= mB.length() ) && mB.substr( mPos, buf.length() ) == buf )
			{
				for( int i = 0; i < buf.length(); ++i ) operator++( 0 );
				return true;
			}
			return false;
		}
		bool accept( int v )
		{
			if ( mB[mPos] == v )
			{
				operator++( 0 );
				return true;
			}
			return false;
		}
		bool match( int v ) {
			return mB[mPos] == v;
		}
		const char *at(int i) { return &mB[i]; }

		ostream& debug( ostream& out )
		{
			out << "Line: " << mLines << " col: " << mCol << " char: " << peek() << " " << (char) peek() << endl;
			return out;
 		}
		string debug()
		{
			stringstream ss;
			ss << "Line: " << mLines << " col: " << mCol << " char: " << peek() << " " << (char) peek();
			return ss.str();
 		}

		protected:
		uint mPos;
		uint mCol;
		uint mLines;
		string mB;

	};

	class JSONToken {
		public:
		JSONToken() {}
		JSONToken( JSONTokenType type ) : mType( type ) {}
		JSONToken( JSONTokenType type, long v ) : mType( type ), ival( v ) {}
		JSONToken( JSONTokenType type, double v ) : mType( type ), dval( v ) {}
		JSONToken( JSONTokenType type, string v ) : mType( type ), sval( v ) {}
		string strVal()
		{
			switch( mType )
			{
				case JSON_STRING: return sval;
				case JSON_IDENTIFIER: return sval;
				case JSON_INT: return to_string( ival );
				case JSON_NUMERIC: return to_string( dval );
				default: return sval;
			}
		}

		static JSONToken INT( JSONTokenType type, long long v )
		{
			JSONToken t( JSON_INT );
			t.ival = v;
			return t;
		}

		static JSONToken UINT( JSONTokenType type, long long v )
		{
			JSONToken t( JSON_UINT );
			t.ival = v;
			return t;
		}

		ostream& print( ostream &out ) const;
		JSONTokenType mType;
		double dval;
		long long ival;
		string sval;
	};

	/* inspration taken from https://github.com/esnme/ultrajson/blob/master/lib/ultrajsondec.c */
	class JSONLexer {

		public:
		JSONLexer( string input ) : buffer ( input ) {}
		bool peek( JSONTokenType type );
		JSONToken peekToken();
		JSONToken getTokenFromBuffer();
		JSONToken getToken();
		bool conditionalGetToken( JSONTokenType type );
		JSONToken peek();
		JSONToken lexString();
		JSONToken lexComment();
		JSONToken lexIdentifier()
		{
			int start = buffer.pos();
			buffer++;
			while ( isIdentChar( buffer.val() ) ) buffer++;
			string tok = buffer.substr(start, buffer.pos() - start );
			if (tok.compare("null") == 0) return JSONToken( JSON_NULL );
			if (tok.compare("true") == 0) return JSONToken( JSON_TRUE );
			if (tok.compare("false") == 0) return JSONToken( JSON_FALSE );
			return JSONToken( JSON_IDENTIFIER, tok );
		}
		JSONToken lexNumeric();
		bool hasExp()
		{
			return buffer.accept('e') || buffer.accept('E');
		}
		JSONToken preciseDouble()
		{
			char *end;
			double value = strtod( buffer.at( mNumStart ), &end );
			return JSONToken( JSON_NUMERIC, value );
		}
		JSONToken advanceExponent() {
			//if( buffer.accept('e') || buffer.accept('E') )
			//{
				if( buffer.accept('+') ) {}
				else if (  buffer.accept('-') )
				{
					mExpNeg = -1;
				}
				mExpStart = buffer.pos();
				advanceDecimalDigits();
				mExpEnd = buffer.pos();
				return preciseDouble();
			//}
		}
		void advanceFraction() {
			if ( buffer.accept('.') ) {
				mFracStart = buffer.pos();
				advanceDecimalDigits();
				mFracEnd = buffer.pos();
			}
		}
		void advanceDecimalDigits()
		{
			acceptDigit();
			while ( isDigit( buffer.val() ) ) buffer++;
		}
		void advanceIntegerLiteral()
		{
			mIntStart = buffer.pos();
			//acceptNonZeroDigit();
			while ( isDigit( buffer.val() ) ) {
				mIntVal = mIntVal * 10ULL + (long) (buffer.val() - 48);
				 buffer++;
			}
			mIntEnd = buffer.pos();
		}
		void optAdvanceDecimalDigits()
		{
			mFracStart = buffer.pos();
			while ( isDigit( buffer.val() ) ) {
				mDecVal = mDecVal * 10ULL + (long) (buffer.val() - 48);
				 buffer++;
			}
			mFracEnd = buffer.pos();
		}
		void acceptDigit()
		{
			if( isDigit( buffer.val() ) )
				buffer++;
			else
				throw "LexError - expected digit";
		}
		void acceptNonZeroDigit()
		{
			if( isnonzero( buffer.val() ) )
			{
				mIntVal = mIntVal * 10ULL + (long) (buffer.val() - 48);
				buffer++;
			}
			else
			{
				throw "LexError - expected non-zero digit";
			}
		}

		string debug()
		{
			stringstream ss;
			buffer.debug( ss );
			return ss.str();
		}
		
		bool isIdentChar( int c )
		{
			return isalnum( c ) || c == '$' || c == '_';
		}
		bool isnonzero( int c )
		{
			switch ( buffer.val() )
			{
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9': return true;
				default: return false;
			}
		}
		bool isWhitespace( int c ) {
			switch (c)
			{
				case ' ':
				case '\t':
				case '\r':
				case '\n': return true;
				default: return false;
			}
		}

		bool isDigit( int c )
		{
			return isdigit( c );
		}
		void skipWhitespace() {
			while ( isWhitespace( buffer.val() ) ) buffer++;
		}

		protected:
		queue< JSONToken > tokens;
		Buffer buffer;
		long long mIntVal;
		long mDecVal;
		int mNumStart;
		int mIntNeg;
		int mIntStart;
		int mIntEnd;
		int mExpNeg;
		int mExpStart;
		int mExpEnd;
		int mFracStart;
		int mFracEnd;
	};


	enum VariantType {
		VARIANT_OBJECT,
		VARIANT_BOOL,
		VARIANT_ARRAY,
		VARIANT_INT,
		VARIANT_UINT,
		VARIANT_DOUBLE,
		VARIANT_STRING,
		VARIANT_INTVEC,
		VARIANT_DOUBLEVEC,
		VARIANT_DOUBLEVEC2,
		VARIANT_NULL
	};

	template< typename MAP >
	typename MAP::mapped_type& upsert( MAP &map, const typename MAP::value_type &_pair )
	{
		pair< typename MAP::iterator, bool > ret = map.emplace ( _pair );
		if ( ret.second == false ) {
			ret.first->second = _pair.second;
		}
		return ret.first->second;
	}

	template< typename MAP >
	typename MAP::mapped_type& upsert( MAP &map, const typename MAP::key_type &key, const typename MAP::mapped_type &value )
	{
		return upsert( map, typename MAP::value_type( key, value ) );
	}

	class JSONObject;
	class JSONToken;
	inline ostream &operator<<( ostream &out, const JSONObject &o);
	inline ostream &operator<<( ostream &out, const JSONToken &t );

	class JSONError
	{
		public:
		JSONError( string  str ) : mDesc( str ) {}
		string mDesc;
	};

	inline ostream& operator<<( ostream& out, const JSONError& e )
	{
		out << e.mDesc;
		return out;
	}

	class JSONObject;
	extern JSONObject JSON_NOTFOUND;
	class JSONObject {
		public:
		typedef map<string, JSONObject> MembersMap;
		typedef MembersMap::value_type MembersMapPair;
		typedef MembersMap::iterator MembersMapI;
		typedef MembersMap::const_iterator MembersMapCI;
		typedef vector< JSONObject> MembersVector;
		typedef MembersVector::const_iterator MembersVecCI;
		JSONObject() : mType( VARIANT_OBJECT ) {} ;
		JSONObject( const JSONObject& other ) = default;
		JSONObject( JSONObject&& other ) = default;
		JSONObject& operator=( const JSONObject& ) = default;
		JSONObject( VariantType s ) : mType( s ) {};
		JSONObject( VariantType s, const JSONObject& o ) : mType( s ) { push_back( o ); };
		JSONObject( VariantType s, const bool b ) : mType( VARIANT_BOOL ), mInt( b ? 1 : 0 ) {  };
		JSONObject( string s ) : mType( VARIANT_STRING ),  mString( s ) {};
		JSONObject( const char* s ) : mType( VARIANT_STRING ),  mString( s ) {};
		JSONObject( unsigned int s ) : mType( VARIANT_UINT ),  mUInt( s ) {};
		JSONObject( int s ) : mType( VARIANT_INT ),  mInt( s ) {};
		JSONObject( unsigned long s ) : mType( VARIANT_UINT ),  mUInt( s ) {};
		JSONObject( long s ) : mType( VARIANT_INT ),  mInt( s ) {};
		JSONObject( unsigned long long s ) : mType( VARIANT_UINT ),  mUInt( s ) {};
		JSONObject( long long s ) : mType( VARIANT_INT ),  mInt( s ) {};
		JSONObject( double s ) : mType( VARIANT_DOUBLE ),  mDouble( s ) {};
		explicit JSONObject( const bool b ) : mType( VARIANT_BOOL ), mInt( b ? 1 : 0 ) {  };
		JSONObject( IntVec s ) : mType( VARIANT_INTVEC ),  mIntVec( s ) {};
		JSONObject( const Vector3d s ) : mType( VARIANT_DOUBLEVEC )
		{
			for( uint i = 0; i < s.size(); ++i )
				mDoubleVec.emplace_back( s( i ) );
		};

		JSONObject( const VectorXd s ) : mType( VARIANT_DOUBLEVEC )
		{
			for( uint i = 0; i < s.size(); ++i )
				mDoubleVec.emplace_back( s( i ) );
		};

		JSONObject( const MatrixXd s ) : mType( VARIANT_DOUBLEVEC )
		{
			mDoubleVec.emplace_back( s.rows() );
			mDoubleVec.emplace_back( s.cols() );
			const int rows = s.rows();
			const int cols = s.cols();
			for( uint i = 0; i < rows; ++i )
			{
				for( uint j = 0; j < cols; ++j )
				{
					mDoubleVec.emplace_back( s( i, j ) );
				}
			}
		};

		JSONObject( const VectorXi s ) : mType( VARIANT_INTVEC )
		{
			for( uint i = 0; i < s.size(); ++i )
				mIntVec.emplace_back( s( i ) );
		};

		static JSONObject boolean( bool b )
		{
			return JSONObject( VARIANT_BOOL, b );
		}
		static JSONObject True()
		{
			return JSONObject( VARIANT_BOOL, true );
		}
		static JSONObject False()
		{
			return JSONObject( VARIANT_BOOL, false );
		}

		static JSONObject Null()
		{
			return JSONObject( VARIANT_NULL );
		}

		JSONObject( const DoubleVec& s ) : mType( VARIANT_DOUBLEVEC ),  mDoubleVec( s ) {};
		JSONObject( const DoubleVec2& s ) : mType( VARIANT_DOUBLEVEC2 ),  mDoubleVec2( s ) {};
		/*
		JSONObject( const KeyValueVec &vec ) {
			for ( KeyValueVecCI i = vec.begin(); i != vec.end(); i++ )
			{
				util::upsert( members, i->first, i->second );
			}
		}
		*/
		JSONObject( const initializer_list<MembersMapPair> list ) : mType( VARIANT_OBJECT ) {
			for( const auto elem : list ) upsert( elem );
		}
		VariantType getType() const { return mType; }

		void erase( const char * key )
		{
			mMembers.erase( key );
		}

		void erase( string& key )
		{
			mMembers.erase( key );
		}

		template<typename T>
		JSONObject& upsert( const string& str, const T o ) { return upsert( str, JSONObject( o ) ); }
		
		template<typename T>
		JSONObject& upsert( const char* str, const T o ) { return upsert( string( str ), JSONObject( o ) ); }
		
		JSONObject& upsert( const string& str, const JSONObject& o = JSONObject() ) {
			pair< MembersMap::iterator, bool > ret = mMembers.emplace( make_pair( str, o ) );
			if ( ret.second == false )
			{
				if( ret.first->second.isArray() )
				{
					ret.first->second.push_back( o );
					return ret.first->second.back();
				}
				else
				{
					return util::upsert( mMembers, str, o );
					// /// make it an array
					// ret.first->second = JSONObject( VARIANT_ARRAY, ret.first->second );
					// ret.first->second.push_back( o );
					// return ret.first->second.back();
				}
			}
			else
				return ret.first->second;
		}

		JSONObject& upsert( const char* str, const JSONObject& o = JSONObject() ) { return upsert( string( str ), o ); }

		/// update with the entries from the input JSONObject.  Any existing values with the same keys in both will be overwritten with the values from the input o.
		void update( const JSONObject& o )
		{
			for( const auto & x : o.mMembers )
				util::upsert( mMembers, x.first, x.second );
		}

		JSONObject& insertArray( string str, const JSONObject& o = JSONObject( VARIANT_ARRAY ) ) {
			pair< MembersMap::iterator, bool > ret = mMembers.emplace( make_pair( str, o ) );
			if ( ret.second == false )
			{
				COUT_THROW_EXCEPTION_AT( IGXException, "JSON member " << str << " already exits." );
			}
			else
				return ret.first->second;
		}

		void upsert( const MembersMapPair &pair ) {
			util::upsert( mMembers, pair.first, pair.second );
		}
		void upsert( const JSONObject& o ) {
			for( const auto & x : o.mMembers )
				util::upsert( mMembers, x.first, x.second );
		}

		void push_back( const JSONObject& o ) {
			switch ( getType() )
			{
				case VARIANT_ARRAY:
				{
					mMembersVec.emplace_back( o );
					break;
				}
				case VARIANT_INTVEC:
				{
					mMembersVec.emplace_back( o.i() );
					break;
				}
				case VARIANT_DOUBLEVEC:
				{
					mMembersVec.emplace_back( o.d() );
					break;
				}
				default:
					COUT_THROW_EXCEPTION_AT( JSONError, "Expected vector" );
			}
		}

		// template<typename T>
		// typename enable_if<! is_same<JSONObject, T>::value, void>::type push_back( const T& o ) { push_back( JSONObject( o ) ); };

		template<typename T>
		typename enable_if<! is_same<JSONObject, typename remove_reference<T>::type>::value, void>::type push_back( const T o ) { push_back( JSONObject( o ) ); };

		JSONObject& back() {
			return mMembersVec.back();
		}
		
		/// Returns false if the path is not present
		bool hasPath( string path ) const {
			const JSONObject &result = resolvePath( path );
			return !( result == JSON_NOTFOUND );
		}

		/// Returns false if the path is not present or the path value is false
		bool hasTrue( string path ) const  {
			const JSONObject &result = resolvePath( path );
			return ( result == JSON_NOTFOUND ) ? false : result.boolean();
		}

		bool hasKeyValue( string path, string val ) const {
			const JSONObject &result = resolvePath( path );
			return ( !( result == JSON_NOTFOUND ) ) && ( result.strVal() == val );
		}
		
		bool hasKeyValue( string path, int val ) const {
			const JSONObject &result = resolvePath( path );
			return ( !( result == JSON_NOTFOUND ) ) && ( result.intVal() == val );
		}

		bool isObject() const { return mType == VARIANT_OBJECT; }
		bool isArray() const { return mType == VARIANT_ARRAY; }
		bool isUINT() const { return mType == VARIANT_UINT; }
		bool isINT() const { return mType == VARIANT_INT; }

		/// Returns a boolean representation of the object
		bool boolean() const {
			switch( mType )
			{
				case VARIANT_BOOL: return mInt != 0;
				case VARIANT_INT: return mInt != 0;
				case VARIANT_DOUBLE: return ! equals( mDouble, 0.0, 1e-8 );
				default: return false;
			}
		}

		size_t size() const
		{ 
			switch( mType )
			{
				case VARIANT_ARRAY: return mMembersVec.size();
				case VARIANT_OBJECT: return mMembers.size();
				case VARIANT_INTVEC: return mIntVec.size();
				case VARIANT_DOUBLEVEC: return mDoubleVec.size();
				case VARIANT_DOUBLEVEC2: return mDoubleVec2.size();
				default: COUT_THROW_EXCEPTION_AT( JSONError, "Expected an array" );
			}
		}

		JSONObject& operator[]( int index )
		{
			switch( mType )
			{
				case VARIANT_ARRAY:
				{
					if( index >= size() )
					{
					COUT_THROW_EXCEPTION_AT( JSONError, "Array index to big" );
					}
					return mMembersVec[ index ];
				}
				case VARIANT_OBJECT:
					return member( to_string( index ) ) ;
					
				default: COUT_THROW_EXCEPTION_AT( JSONError, "Expected an array" );
			}
		}
		
		const JSONObject& operator[]( int index ) const
		{
			switch( mType )
			{
				case VARIANT_ARRAY:
				{
					if( index >= size() )
					{
						COUT_THROW_EXCEPTION_AT( JSONError, "Array index to big" );
					}
					return mMembersVec[ index ];
				}
				case VARIANT_OBJECT:
					return member( to_string( index ) ) ;
					
				default: COUT_THROW_EXCEPTION_AT( JSONError, "Expected an array" );
			}
		}
		
		JSONObject& operator[]( string path )
		{
			JSONObject &result = resolvePath( path );
			if( result == JSON_NOTFOUND ) COUT_THROW_EXCEPTION_AT( JSONError, "Expected object at path " << path );
			return result;
		}

		const JSONObject& operator[]( string path ) const
		{
			const JSONObject &result = resolvePath( path );
			if( result == JSON_NOTFOUND ) COUT_THROW_EXCEPTION_AT( JSONError, "Expected object at path " << path );
			return result;
		}

		/// must find JSONobject at path or throws exception
		const JSONObject& orThrow( string path ) const {
			const JSONObject &result = resolvePath( path );
			if( result == JSON_NOTFOUND ) COUT_THROW_EXCEPTION_AT( JSONError, "Expected object at path " << path );
			return result;
		}
		
		/// must find JSONobject at path or throws exception
		const JSONObject& objectOrThrow( string path ) const {
			const JSONObject &result = resolvePath( path );
			if( result == JSON_NOTFOUND || ! result.isObject() ) COUT_THROW_EXCEPTION_AT( JSONError, "Expected object at path " << path );
			return result;
		}
		
		const JSONObject& arrayOrThrow( string path ) const {
			const JSONObject &result = resolvePath( path );
			if( result == JSON_NOTFOUND || ! result.isArray() ) COUT_THROW_EXCEPTION_AT( JSONError, "Expected array at path " << path );
			return result;
		}

		/// must find JSONobject at path with key sub_path and value val or throws exception
		const JSONObject& objectOrThrow( string path, string sub_path, int val ) const {
			const JSONObject &result = resolvePath( path );
			if( result == JSON_NOTFOUND || ! ( result.isObject() || result.isArray() ) ) COUT_THROW_EXCEPTION_AT( JSONError, "Expected object at path " << path );
			switch( result.mType )
			{
				case VARIANT_ARRAY:
					for( auto& x : mMembersVec )
					{
						if( x.hasKeyValue( sub_path, val ) )
							return x;
					}
					COUT_THROW_EXCEPTION_AT( JSONError, "Expected object/array at path " << path );
					break;
				case VARIANT_OBJECT:
					if( result.hasKeyValue( sub_path, val ) )
						return result;
					else
						COUT_THROW_EXCEPTION_AT( JSONError, "Expected object/array at path " << path );
					break;
				default:
					COUT_THROW_EXCEPTION_AT( JSONError, "Expected object/array at path " << path );
			}
		}

		void paths( string path, function < void ( const JSONObject& x ) > callback ) const
		{
			const JSONObject &result = resolvePath( path );
			switch( result.mType )
			{
				case VARIANT_ARRAY:
					for( auto& x : result.mMembersVec )
						callback( x );
					break;
				case VARIANT_OBJECT:
					callback( result );
				default:
					COUT_THROW_EXCEPTION_AT( JSONError, "Expected object/array at path " << path );
			}
			
		}
		
		const MembersVector& array( string path ) const
		{
			const JSONObject &result = resolvePath( path );
			switch( result.mType )
			{
				case VARIANT_ARRAY:
					return result.mMembersVec;
					break;
				default:
					COUT_THROW_EXCEPTION_AT( JSONError, "Expected array at path " << path );
			}
			
		}

		void iterate( const string& path, function< void ( const JSONObject& ) > callback ) const {
			const JSONObject &result = resolvePath( path );
			switch( result.mType )
			{
				case VARIANT_ARRAY:
					for( auto& x : result.mMembersVec )
					{
						callback( x );
					}
					break;
				default:
					COUT_THROW_EXCEPTION_AT( JSONError, "Expected array at path " << path );
			}
		}

		void iterate( function< void ( const JSONObject& ) > callback ) const {
			switch( mType )
			{
				case VARIANT_ARRAY:
					for( auto& x : mMembersVec )
					{
						callback( x );
					}
					break;
				default:
					COUT_THROW_EXCEPTION_AT( JSONError, "Expected array" );
			}
		}

		void arrayPathInsert( IntVec array_path, const MatrixXd& m )
		{
			JSONObject* cur = this;
			for( int i = 0, e = array_path.size(); i < e; ++i )
			{
				if ( i != e - 1 )
				{
					pair< MembersMap::iterator, bool > ret = cur->mMembers.emplace( make_pair( to_string( array_path[ i ] ), JSONObject() ) );
					cur = &ret.first->second;
				}
				else
				{
					pair< MembersMap::iterator, bool > ret = cur->mMembers.emplace( make_pair( to_string( array_path[ i ] ), JSONObject( VARIANT_ARRAY ) ) );
					cur = &ret.first->second;
					for( int j = 0 ; j < m.cols(); ++j )
					{
						cur->push_back( JSONObject( VectorXd( m.col( j ) ) ) );
					}
				}
			}
		}
		
		string s( string path ) const {
			const JSONObject &result = resolvePath( path );
			if( result == JSON_NOTFOUND ) COUT_THROW_EXCEPTION_AT( JSONError, "Expected object at path " << path );
			return result.strVal();
		}

		string s( string path, const string default_val ) const {
			const JSONObject &result = resolvePath( path );
			return ( result == JSON_NOTFOUND ) ? default_val : result.strVal();
		}

		double d( string path ) const {
			const JSONObject &result = resolvePath( path );
			if( result == JSON_NOTFOUND ) COUT_THROW_EXCEPTION_AT( JSONError, "Expected object at path " << path );
			return result.dblVal();
		}

		double d( string path, const double default_val ) const {
			const JSONObject &result = resolvePath( path );
			return ( result == JSON_NOTFOUND ) ? default_val : result.dblVal();
		}

		int b( string path ) const {
			const JSONObject &result = resolvePath( path );
			if( result == JSON_NOTFOUND ) COUT_THROW_EXCEPTION_AT( JSONError, "Expected object at path " << path );
			return result.boolean();
		}

		int b( string path, const bool default_val ) const {
			const JSONObject &result = resolvePath( path );
			return ( result == JSON_NOTFOUND ) ? default_val : result.boolean();
		}

		int i( string path ) const {
			const JSONObject &result = resolvePath( path );
			if( result == JSON_NOTFOUND ) COUT_THROW_EXCEPTION_AT( JSONError, "Expected object at path " << path );
			return result.intVal();
		}

		int i( string path, const int default_val ) const {
			const JSONObject &result = resolvePath( path );
			return ( result == JSON_NOTFOUND ) ? default_val : result.intVal();
		}

		uint u( string path ) const {
			const JSONObject &result = resolvePath( path );
			if( result == JSON_NOTFOUND ) { COUT_THROW_EXCEPTION_AT( JSONError, "Expected object at path " << path ); }
			else if( result.isUINT()) return result.uintVal();
			else return result.intVal();
		}

		uint u( string path, const uint default_val ) const {
			const JSONObject &result = resolvePath( path );
			if( result == JSON_NOTFOUND ) return default_val;
			else if( result.isUINT()) return result.uintVal();
			else return result.intVal();
		}

		IntVec iv( string path ) const {
			const JSONObject &result = resolvePath( path );
			if( result == JSON_NOTFOUND ) COUT_THROW_EXCEPTION_AT( JSONError, "Expected object at path " << path );
			switch ( result.mType )
			{
				case VARIANT_ARRAY:
				{
					IntVec v;
					for( auto& x : result.mMembersVec )
					{
						v.push_back( x.intVal() );
					}
					return v;
				}
				case VARIANT_INTVEC:
					return result.mIntVec;
				case VARIANT_DOUBLE:
				{
					IntVec v;
					for( auto& x : result.mDoubleVec )
					{
						v.push_back( x );
					}
					return v;
				}
				default:
					COUT_THROW_EXCEPTION_AT( JSONError, "Expected vector at path " << path );
			}
		}

		IntVec iv( string path, const IntVec &default_val ) const {
			const JSONObject &result = resolvePath( path );
			if ( result == JSON_NOTFOUND ) return default_val;
			switch ( result.mType )
			{
				case VARIANT_ARRAY:
				{
					IntVec v;
					for( auto& x : result.mMembersVec )
					{
						v.push_back( x.intVal() );
					}
					return v;
				}
				case VARIANT_INTVEC:
					return result.mIntVec;
				case VARIANT_DOUBLE:
				{
					IntVec v;
					for( auto& x : result.mDoubleVec )
					{
						v.push_back( x );
					}
					return v;
				}
				default:
					COUT_THROW_EXCEPTION_AT( JSONError,  "Expected vector at path " << path );
			}
		}

		UIntVec uv( string path ) const {
			const JSONObject &result = resolvePath( path );
			if( result == JSON_NOTFOUND ) COUT_THROW_EXCEPTION_AT( JSONError,  "Expected object at path " << path );
			switch ( result.mType )
			{
				case VARIANT_ARRAY:
				{
					UIntVec v;
					for( auto& x : result.mMembersVec )
					{
						v.push_back( x.uintVal() );
					}
					return v;
				}
				case VARIANT_INTVEC:
				{
					UIntVec v;
					for( auto& x : result.mIntVec )
					{
						v.push_back( x );
					}
					return v;
				}
				case VARIANT_DOUBLE:
				{
					UIntVec v;
					for( auto& x : result.mDoubleVec )
					{
						v.push_back( x );
					}
					return v;
				}
				default:
					COUT_THROW_EXCEPTION_AT( JSONError,  "Expected vector at path " << path );
			}
		}

		UIntVec uv( string path, const UIntVec &default_val ) const {
			const JSONObject &result = resolvePath( path );
			if ( result == JSON_NOTFOUND ) return default_val;
			switch ( result.mType )
			{
				case VARIANT_ARRAY:
				{
					UIntVec v;
					for( auto& x : result.mMembersVec )
					{
						v.push_back( x.uintVal() );
					}
					return v;
				}
				case VARIANT_INTVEC:
				{
					UIntVec v;
					for( auto& x : result.mIntVec )
					{
						v.push_back( x );
					}
					return v;
				}
				case VARIANT_DOUBLE:
				{
					UIntVec v;
					for( auto& x : result.mDoubleVec )
					{
						v.push_back( x );
					}
					return v;
				}
				default:
					COUT_THROW_EXCEPTION_AT( JSONError,  "Expected vector at path " << path );
			}
		}

		string strVal() const
		{
			switch ( mType )
			{
				case VARIANT_STRING: return mString;
				case VARIANT_DOUBLE: return to_string( mDouble );
				case VARIANT_INT: return to_string( mInt );
				default:
					COUT_THROW_EXCEPTION_AT( JSONError, "No valid string value" );
			}
		}
		string s() const { return strVal(); }

		int intVal() const
		{
			switch ( mType )
			{
				case VARIANT_DOUBLE: return mDouble;
				case VARIANT_INT: return mInt;
				case VARIANT_STRING: return atoi( mString.c_str() );
				default:
					COUT_THROW_EXCEPTION_AT( JSONError, "No valid int value" );
			}
		}
		int i() const { return intVal(); }
		
		uint uintVal() const
		{
			switch ( mType )
			{
				case VARIANT_INT:
				{
					if( mInt < 0 )
						COUT_THROW_EXCEPTION_AT( JSONError, "No valid int value (cannot convert negative int to uint)" );
					return mInt;
				}
				case VARIANT_UINT: return mUInt;
				default:
					COUT_THROW_EXCEPTION_AT( JSONError, "No valid uint value" );
			}
		}
		uint u() const { return intVal(); }
		
		double dblVal() const
		{
			switch ( mType )
			{
				case VARIANT_DOUBLE: return mDouble;
				case VARIANT_INT: return mInt;
				case VARIANT_STRING: return strtod( mString.c_str(), NULL );
				default:
					COUT_THROW_EXCEPTION_AT( JSONError, "No valid double value" );
			}
		}
		double d() const { return dblVal(); }
		 

		DoubleVec dv( string path ) const
		{
			const JSONObject &result = resolvePath( path );
			if( result == JSON_NOTFOUND ) COUT_THROW_EXCEPTION_AT( JSONError,  "Expected object at path " << path );
			switch ( result.mType )
			{
				case VARIANT_ARRAY:
				{
					DoubleVec v;
					for( auto& x : result.mMembersVec )
					{
						v.push_back( x.dblVal() );
					}
					return v;
				}
				case VARIANT_INTVEC:
				{
					DoubleVec v;
					for( auto& x : result.mIntVec )
					{
						v.push_back( x );
					}
					return v;
				}
				case VARIANT_DOUBLE:
					return result.mDoubleVec;
				default:
					COUT_THROW_EXCEPTION_AT( JSONError,  "Expected vector at path " << path );
			}
		}
		
		DoubleVec dv( string path, const DoubleVec &default_val ) const
		{
			const JSONObject &result = resolvePath( path );
			if ( result == JSON_NOTFOUND ) return default_val;
			switch ( result.mType )
			{
				case VARIANT_ARRAY:
				{
					DoubleVec v;
					for( auto& x : result.mMembersVec )
					{
						v.push_back( x.dblVal() );
					}
					return v;
				}
				case VARIANT_INTVEC:
				{
					DoubleVec v;
					for( auto& x : result.mIntVec )
					{
						v.push_back( x );
					}
					return v;
				}
				case VARIANT_DOUBLE:
					return result.mDoubleVec;
				default:
					COUT_THROW_EXCEPTION_AT( JSONError,  "Expected vector at path " << path );
			}
		}
		
		Vector6d toVector6d() const
		{
			switch ( mType )
			{
				case VARIANT_ARRAY:
				{
					Vector6d v;
					for( int i = 0; i < mMembersVec.size(); ++i )
						v( i ) = mMembersVec[ i ].dblVal();
					return v;
				}
				case VARIANT_INTVEC:
				{
					Vector6d v;
					for( int i = 0; i < mIntVec.size(); ++i )
						v( i ) = mIntVec[ i ];
					return v;
				}
				case VARIANT_DOUBLEVEC:
				{
					Vector6d v;
					for( int i = 0; i < mDoubleVec.size(); ++i )
						v( i ) = mDoubleVec[ i ];
					return v;
				}
				default:
					COUT_THROW_EXCEPTION_AT( JSONError, "Expected vector" );
			}
		}
		Vector6d vector6d( string path ) const
		{
			const JSONObject &result = resolvePath( path );
			if( result == JSON_NOTFOUND ) COUT_THROW_EXCEPTION_AT( JSONError, "Expected object at path " << path );
			return result.toVector6d();
		}

		Vector6d vector6d( string path, const Vector6d &default_val ) const
		{
			const JSONObject &result = resolvePath( path );
			if ( result == JSON_NOTFOUND ) return default_val;
			return result.toVector6d();
		}

		Vector3d toVector3d() const
		{
			switch ( mType )
			{
				case VARIANT_ARRAY:
				{
					Vector3d v;
					for( int i = 0; i < mMembersVec.size(); ++i )
						v( i ) = mMembersVec[ i ].dblVal();
					return v;
				}
				case VARIANT_INTVEC:
				{
					Vector3d v;
					for( int i = 0; i < mIntVec.size(); ++i )
						v( i ) = mIntVec[ i ];
					return v;
				}
				case VARIANT_DOUBLEVEC:
				{
					Vector3d v;
					for( int i = 0; i < mDoubleVec.size(); ++i )
						v( i ) = mDoubleVec[ i ];
					return v;
				}
				default:
					COUT_THROW_EXCEPTION_AT( JSONError,  "Expected vector" );
			}
		}

		
		Vector3d vector3d( string path ) const
		{
			const JSONObject &result = resolvePath( path );
			if( result == JSON_NOTFOUND ) COUT_THROW_EXCEPTION_AT( JSONError,  "Expected object at path " << path );
			return result.toVector3d();
		}

		Vector3d vector3d( string path, const Vector3d &default_val ) const
		{
			const JSONObject &result = resolvePath( path );
			if ( result == JSON_NOTFOUND ) return default_val;
			return result.toVector3d();
		}

		//template<typename VECTOR>
		VectorXd toVectorXd() const
		{
			VectorXd v( size() );
			switch ( getType() )
			{
				case VARIANT_ARRAY:
				{
					for( int i = 0; i < mMembersVec.size(); ++i )
						v( i ) = mMembersVec[ i ].dblVal();
					break;
				}
				case VARIANT_INTVEC:
				{
					for( int i = 0; i < mIntVec.size(); ++i )
						v( i ) = mIntVec[ i ];
					break;
				}
				case VARIANT_DOUBLEVEC:
				{
					for( int i = 0; i < mDoubleVec.size(); ++i )
						v( i ) = mDoubleVec[ i ];
					break;
				}
				default:
					COUT_THROW_EXCEPTION_AT( JSONError, "Expected vector" );
			}
			return v;
		}

		VectorXd vectorXd( string path ) const
		{
			const JSONObject &result = resolvePath( path );
			if( result == JSON_NOTFOUND ) COUT_THROW_EXCEPTION_AT( JSONError, "Expected object at path " << path );
			return result.toVectorXd();
		}

		VectorXd vectorXd( string path, const Vector3d &default_val ) const
		{
			const JSONObject &result = resolvePath( path );
			if( result == JSON_NOTFOUND ) return default_val;
			return result.toVectorXd();
		}

		MaybeRef< const JSONObject > maybeArray( const string & path) const
		{
			const JSONObject &result = resolvePath( path );
			if( result == JSON_NOTFOUND || ! result.isArray() ) return NothingRef< const JSONObject>();
			return result;
		}

		Maybe< const VectorXd > maybeVectorXd( const string & path) const
		{
			const JSONObject &result = resolvePath( path );
			if( result == JSON_NOTFOUND ) return Nothing< const VectorXd >();
			return Just< const VectorXd >( result.toVectorXd() );
		}

		static JSONObject fromFile( string filename, const bool debug = false );

		const JSONObject& resolvePath( const string &path ) const
		{
			StringVec keys = split( path, '.' );
			return resolvePath( keys.begin(), keys.end() );
		}

		JSONObject& resolvePath( const string &path )
		{
			StringVec keys = split( path, '.' );
			return resolvePath( keys.begin(), keys.end() );
		}

		const JSONObject& resolvePath( const StringVecCI &begin, const StringVecCI &end ) const;

		JSONObject& resolvePath( const StringVecCI &begin, const StringVecCI &end );

		const JSONObject& member( const string &path ) const;

		JSONObject& member( const string &path );

		ostream& outTabWidth( ostream& out, const int ident ) const
		{
			for( int j = 0; j < ident; ++j ) out << "  ";
			return out;
		}

		// output JSON with formatting
		void outline( ostream &out, int ident = 0 ) const;

		bool operator==( const JSONObject &other ) const {
			return this == &other;
		}

		static JSONObject parse( string s );

		void iterateKeys( std::function<void( string )> callback ) const
		{
			for( const auto & x : mMembers )
				callback( x.first );
		}
		void iterateArray( std::function<void( const JSONObject&  )> callback ) const
		{
			switch ( getType() )
			{
				case VARIANT_ARRAY:
				{
					for( const auto & x : mMembersVec )
						callback( x );
					break;
				}
				case VARIANT_INTVEC:
				{
					for( const auto & x : mIntVec )
						callback( JSONObject( x ) );
					break;
				}
				case VARIANT_DOUBLEVEC:
				{
					for( const auto & x : mDoubleVec )
						callback( JSONObject( x ) );
					break;
				}
				default:
					COUT_THROW_EXCEPTION_AT( JSONError, "Expected vector" );
			}
		}
		void iterateObject( std::function<void( const string&, const JSONObject&  )> callback ) const
		{
			for( const auto & x : mMembers )
				callback( x.first, x.second );
		}

		protected:
		VariantType mType;
		map<string, JSONObject > mMembers;
		vector< JSONObject > mMembersVec;
		public:
		double mDouble;
		string mString;
		long mInt;
		uint mUInt;
		IntVec mIntVec;
		DoubleVec mDoubleVec;
		DoubleVec2 mDoubleVec2;
	};
	
	inline ostream &operator<<( ostream &out, const JSONObject &o) {
		o.outline( out );
		return out;
	}

	typedef pair< string, JSONObject > KeyValue;
	typedef vector< KeyValue> KeyValueVec;
	typedef vector< KeyValue>::const_iterator KeyValueVecCI;
	typedef map<string, JSONObject> MembersMap;
	typedef vector< JSONObject> MembersVector;

	class JSONDumper {
		public:
		JSONDumper ( ) : mOut( mSS )
		{
			outln( "[" );
		}
		JSONDumper ( string filename ) : mFilename( filename ), mOut( mFS )
		{
			mFS.open( filename.c_str(), std::ofstream::out );
			outln( "[" );
		}

		void item( const string &o ) { item( JSONObject::parse( o ) ); }
		void item( const char* o ) { item( JSONObject::parse( o ) ); }
		//void item( const KeyValueVec &o ) { item( JSONObject( o ) ); }
		void item( const JSONObject &o ) {
			o.outline( mOut );
			mOut << "," << endl;
			mOut.flush();
		}
		void item( const initializer_list<JSONObject::MembersMapPair> list )
		{
			item( JSONObject( list ) );
		}

		void close ( const string &str ) { outln( "]" ); mFS.close(); }
		JSONDumper &operator <<( const char* o )
		{
			*this << JSONObject::parse( o ); return *this;
		}
		JSONDumper &operator <<( const string &o )
		{
			*this << JSONObject::parse( o ); return *this;
		}
		/*
		JSONDumper &operator <<( const KeyValueVec &o )
		{
			*this << JSONObject( o ); return *this;
		}
		*/
		JSONDumper &operator <<( const JSONObject &o )
		{
			mOut << o;
			return *this;
		}
		string asString() { return mSS.str() + "]"; }
		protected:
		template< typename T >
		void dump( const T& ) {

		}
		void outln( const string &str ) { mOut << str << endl; }

		string mFilename;
		stringstream mSS;
		fstream mFS;
		ostream &mOut;
	};

	class JSONParser {
		public:
		JSONParser( string data, const bool debug = false ) : lex( data ), mDebug( debug ) {}
		static JSONObject parseFile( string filename, const bool debug = false );
		static JSONObject parse( string data );
		JSONObject parseObject();
		void parseLiteral( JSONTokenType type );

		protected:
		JSONLexer lex;
		bool mDebug = false;
	};

	inline ostream &operator<<( ostream &out, const JSONToken &t )
	{
		return t.print( out );
	}
};
#endif
