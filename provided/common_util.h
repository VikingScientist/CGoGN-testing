#ifndef _UTIL_COMMON_UTIL_H_
#define _UTIL_COMMON_UTIL_H_

#include <climits>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <cmath>
#include <cstring>
#include <string>
#include <assert.h>
#include <stdio.h>

#include <Eigen/Dense>

#include <IGXColors.h>
#include <IGXException.h>
#include <Backtrace.h>

#ifdef __MINGW32__
typedef _off64_t off64_t;
#endif
#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif
#include <time.h>
#ifdef _WIN32
typedef int int32_t;
#endif
#ifdef _WIN32
#define IGA_ALIGN_16 __declspec(align(32))
#else
#define IGA_ALIGN_16 __attribute__(( aligned(16) ))  
#endif

#include <FastVector.h>

#if defined(__GNUC__)
#define DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(__clang__)
#define DEPRECATED(func) func __attribute__((deprecated("Deprecated")))
#elif defined(_MSC_VER)
#define DEPRECATED(func) __declspec(deprecated) func
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED(func) func
#endif

typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned short ushort;
typedef unsigned char uchar;

//#define cout cout << __FILE__ << ":" << __LINE__ << " "

namespace util
{
	using namespace Eigen;
	using namespace std;

	/// returns true if integer argument is odd
	inline bool odd( const int i ) { return i % 2; }
	/// returns true if integer argument is even
	inline bool even( const int i ) { return ( i % 2 ) == 0; }

	/// MIKE Tolerance these
	inline bool neg( const double x )
	{
		return ( x < 0.0 );
	}

	inline bool pos( const double x )
	{
		return ( x >= 0.0 );
	}

	typedef std::vector< double > DoubleVec;
	typedef std::vector< DoubleVec > DoubleVecVec;

	/// This form is a little inconvenient, but is the basis of most other
	/// ways of measuring equality.
	inline bool equals( double a, double b, double tolerance )
	{
		// This method has been benchmarked, and it's pretty fast.
		return ( a == b ) ||
			( ( a <= ( b + tolerance ) ) &&
			  ( a >= ( b - tolerance ) ) );
	}

	template< typename TYPE, int ROW, int COL >
	inline bool equals( const Matrix< TYPE, ROW, COL >& a,
			    const Matrix< TYPE, ROW, COL >& b,
			    const double tol )
	{
		// This method has been benchmarked, and it's pretty fast.
		const double* aa = a.data();
		const double* bb = b.data();
		for( uint i = 0; i < a.size(); ++i )
		{
			if( abs( aa[ i ] - bb[ i ] ) > tol )
				return false;
		}
		return true;
	}

	template< typename TYPE, int ROW, int COL >
	inline bool notEquals( const Matrix< TYPE, ROW, COL >& a,
			       const Matrix< TYPE, ROW, COL >& b,
			       const double tol )
	{
		return !equals( a, b, tol );
	}

	template< class TYPE, int ROW, int COL >
	inline bool lessThan( const Matrix< TYPE, ROW, COL >& a,
			      const Matrix< TYPE, ROW, COL >& b,
			      const double tol )
	{
		// This method has been benchmarked, and it's pretty fast.
		const double* aa = a.data();
		const double* bb = b.data();
		for( uint i = 0; i < a.size(); ++i )
		{
			if( not ( aa[ i ] < bb[ i ] - tol ) )
				return false;
		}
		return true;
	}

	template< typename TYPE, int ROW, int COL >
	inline bool greaterThan( const Matrix< TYPE, ROW, COL >& a,
				 const Matrix< TYPE, ROW, COL >& b,
				 const double tol )
	{
		// This method has been benchmarked, and it's pretty fast.
		const double* aa = a.data();
		const double* bb = b.data();
		for( uint i = 0; i < a.size(); ++i )
		{
			if( not ( aa[ i ] > bb[ i ] + tol ) )
				return false;
		}
		return true;
	}

	template< typename TYPE, int ROW, int COL >
	inline bool lessThanEq( const Matrix< TYPE, ROW, COL >& a,
				const Matrix< TYPE, ROW, COL >& b,
				const double tol )
	{
		!greaterThan( a, b, tol );
	}

	template< typename TYPE, int ROW, int COL >
	inline bool greaterThanEq( const Matrix< TYPE, ROW, COL >& a,
				   const Matrix< TYPE, ROW, COL >& b,
				   const double tol )
	{
		!lessThan( a, b, tol );
	}

	inline bool equals( MatrixXd a, MatrixXd b, double tolerance )
	{
		// This method has been benchmarked, and it's pretty fast.
		const double* aa = a.data();
		const double* bb = b.data();
		for( uint i = 0; i < a.size(); ++i )
			if( abs( aa[ i ] - bb[ i ] ) > tolerance )
				return false;
		return true;
	}

	inline bool equals( const vector< int > &v1, const vector< int > &v2 )
	{
		for( uint i=0; i < v1.size(); i++ )
		{
			if ( v1[i] != v2[i] )
				return false;
		}
		return true;
	}

	template< typename T >
        inline bool equals( const vector< T > &v1, const vector< T > &v2, double tol = 1e-8 )
	{
		if( v1.size() != v2.size() ) return false;
		for( uint i=0; i < v1.size(); i++ )
		{
			if ( !equals(v1[i], v2[i], tol) )
				return false;
		}
		return true;
	}

	template< typename T >
        inline bool equals( const vector< T > &v1, const T* &v2, double tol = 1e-8 )
	{
		for( uint i=0; i < v1.size(); i++ )
		{
			if ( !equals(v1[i], v2[i], tol) )
				return false;
		}
		return true;
	}

	template< typename T >
        inline bool equals( const T* v1, const T* v2, const int size, double tol = 1e-8 )
	{
		for( uint i=0; i < size; i++ )
		{
			if ( !equals(v1[i], v2[i], tol) )
				return false;
		}
		return true;
	}

	template< typename T >
        inline bool equals( const vector< vector< T > > &v1, const vector< vector< T > > &v2, double tol = 1e-8 )
	{
		if( v1.size() != v2.size() ) return false;
		for( uint i=0; i < v1.size(); i++ )
		{
			if ( !equals(v1[i], v2[i], tol) )
				return false;
		}
		return true;
	}

	/// The inverse of the 'equals' operation above, re-expressed for speed.
	inline bool notEquals( double a, double b, double tolerance )
	{
		// The same thing as abs(a-b) > tolerance, but without the branch
		// required by an absolute value call.
		return ( (a-b) > tolerance || (a-b) < -tolerance );
	}

	/// Returns true if a is less than (and not-equal to) b within tolerance.
	inline bool lessThan( double a, double b, double tolerance )
	{
		return a < b - tolerance;
	}

	/// Returns true if a is greater than (and not-equal to) b within tolerance.
	inline bool greaterThan( double a, double b, double tolerance )
	{
		return a > b + tolerance;
	}

	/// Returns true if a is less than or equal to b within tolerance.
	inline bool lessThanEq( double a, double b, double tolerance )
	{
		return !greaterThan( a, b, tolerance );
	}

	/// Returns true if a is greater than or equal to b within tolerance.
	inline bool greaterThanEq( double a, double b, double tolerance )
	{
		return !lessThan( a, b, tolerance );
	}

	template< typename T >
        inline vector< T > subtract( const vector< T > &v1, const vector< T > &v2 )
	{
		if( v1.size() != v2.size() ) exit_message( string ("vector< T > subtract sizes unequal ") + to_string( v1.size() ) + " " + to_string( v2.size() ) );
		vector< T > result;
		for( uint i=0; i < v1.size(); i++ )
		{
			result.push_back( v1[i] - v2[i] );
		}
		return result;
	}

	template< typename T >
        inline vector< vector< T > > subtract( const vector< vector< T > > &v1, const vector< vector< T > > &v2 )
	{
		if( v1.size() != v2.size() ) exit_message( string ("vector< vector< T > > subtract sizes unequal ") + to_string( v1.size() ) + " " + to_string( v2.size() ) );
		vector< vector< T > > result;
		for( uint i=0; i < v1.size(); i++ )
		{
			result.push_back( subtract( v1[i], v2[i] ) );
		}
		return result;
	}


	template< typename T >
        inline bool equals( const vector< T > &v1, const FastDoubleVec &v2, double tol = 1e-8 )
	{
		if( v1.size() != v2.size() ) return false;
		for( uint i=0; i < v1.size(); i++ )
		{
			if ( !util::equals(v1[i], v2[i], tol) )
				return false;
		}
		return true;
	}

	template< typename T >
        inline bool equals( const FastDoubleVec &v1, const vector< T > &v2, double tol = 1e-8 )
	{
		if( v1.size() != v2.size() ) return false;
		for( uint i=0; i < v1.size(); i++ )
		{
			if ( !equals(v1[i], v2[i], tol) )
				return false;
		}
		return true;
	}

	inline bool equals( const FastDoubleVec &v1, const FastDoubleVec &v2, double tol = 1e-8 )
	{
		if( v1.size() != v2.size() ) return false;
		for( uint i=0; i < v1.size(); i++ )
		{
			if ( !util::equals(v1[i], v2[i], tol) )
				return false;
		}
		return true;
	}

	inline bool isZero( double v, double tol = 1e-8 )
	{
		return util::equals( v, 0.0, tol );
	}

	template< class T > vector< T >& clearVec( vector< T > &v, size_t size )
	{
		v.clear();
		v.reserve( size );
		return v;
	}

	template< class T > vector< T >& zeroVec( vector< T > &v, size_t size, const T& val = 0.0 )
	{
		v.clear();
		v.resize( size, val );
		return v;
	}

	template< class T > vector< T >& freeVecInternalMemory( vector< T > &v )
	{
		vector< T > tmp;
		v.swap( tmp );
		return v;
	}

	template< class T > vector< T >& fillVec( vector< T > &v, size_t size, const T& val )
	{
		v.clear();
		v.resize( size, val );
		return v;
	}



	/// Wraps values, positive or negative, around a boundary in
	/// an efficient manner. Useful for rotation calculations. For
	/// example, numWrap( 5, 4 ) is 1, and numWrap( -1, 4 ) is
	/// 3. Similar to the modulus operation (%), but with different
	/// behavior on negative numbers.
	/// Note: The type T can be any integral type, but boundary
	/// should always be positive or the result is undefined.
	template< typename T >
		inline int numWrap( int value, T boundary )
	{
		if( boundary == 0 )
			return 0;
		value %= static_cast< int >( boundary );
		if( value < 0 )
			return static_cast< int >( boundary ) + value;
		else
			return value;
	}

	/// A more generic version of numWrap, used to wrap an
	/// arbitrary value to an arbitrary range. This will wrap
	/// 'value' such that minimum <= value < maximum, and works
	/// on both floating point and integral types. The length
	/// of the range wrapped to will be (maximum - minimum).
	template< typename T >
		inline T numWrap( T value, T minimum, T maximum )
	{
		T range = maximum - minimum;
		if( range == 0.0 )
			return minimum;
		int steps = (int) ((value - minimum) / range);
		T result = value - ((T)steps * range);
		// result should now be either in range or
		// within one range length of being in range.
		if( result < minimum )
			result += range;
		if( result >= maximum )
			result -= range;
		return result;
	}

	/// Clamps the values to a determined range. The values
	/// 'minimum' and 'maxmimum' must be of a type that can be
	/// cast to the same type as 'value', and must be less-than
	/// comparable with value's type as well.
	template< typename T, typename T2, typename T3 >
		inline T numClamp( T value, T2 minimum, T3 maximum )
	{
		if( value < minimum )
			return minimum;
		if( maximum < value )
			return maximum;
		return value;
	}

	/// Returns the sign of the passed number as a double. -1.0 for negative
	/// numbers, 1.0 for positive numbers, and 0.0 if it's equal to zero.
	/// I mostly wrote this because it makes some if() statements easier to write.
	inline int sign( double value )
	{
		if( value > 0.0 )
			return 1;
		if( value < 0.0 )
			return -1;
		return 0;
	}

	/// The faster way to compute fabs.	 The fabs function is about 2 orders of magnitude slower than
	/// this function--it's probably doing some odd checks and handling weird cases.
	inline double fastfabs( double value )
	{
		return ( value < 0.0 ? -value : value );
	}

	/// Clamps to the input range of acos, so the results are always usable,
	/// even in the case of floating point noise.
	inline double clamped_acos( double value )
	{
		assert( value >= (-1 - 1e-8) && value <= (1 + 1e-8) );
		return acos( numClamp( value, -1.0, 1.0 ) );
	}

	inline uint factorial( const uint n )
	{
		uint fac = 1;
		for( uint i = 1; i <= n; ++i )
			fac *= i;
		return fac;
	}

	static int binomials[]
	{
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 3, 3, 1, 0, 0, 0, 0, 0, 0, 0,
			1, 4, 6, 4, 1, 0, 0, 0, 0, 0, 0,
			1, 5, 10, 10, 5, 1, 0, 0, 0, 0, 0,
			1, 6, 15, 20, 15, 6, 1, 0, 0, 0, 0,
			1, 7, 21, 35, 35, 21, 7, 1, 0, 0, 0,
			1, 8, 28, 56, 70, 56, 28, 8, 1, 0, 0,
			1, 9, 36, 84, 126, 126, 84, 36, 9, 1, 0,
			1, 10, 45, 120, 210, 252, 210, 120, 45, 10, 1
			};

	inline double binomialCoefficient( double n, double k )
	{
		if( n <= 10 ) return binomials[((int)n * 11) + (int)k];

		double c;
		if ((k < 0) || (k > n))
			return 0;
		if (k > n - k)
			k = n - k;
		c = 1;
		for (int i=1; i<=k; i++)
			c = c * (n - (k - i)) / i;
		return c;
	}

	/// Transforms the container into a boolean vector.
	template< class Container >
		inline std::vector< bool > asVectorBool( const Container &items, uint capacity )
	{
		std::vector< bool > ret( capacity, false );
		for( typename Container::const_iterator i = items.begin(); i != items.end(); ++i )
			ret[*i] = true;
		return ret;
	}

	/// Transforms the vector into a set.
	template< class T >
		inline std::set< T > asSet( const std::vector< T > &items )
	{
		std::set< T > ret;
		for( typename std::vector< T >::const_iterator i = items.begin(); i != items.end(); ++i )
			ret.insert( *i );
		return ret;
	}

	/// Transforms the set into a vector.
	template< class T >
		inline std::vector< T > asVector( const std::set< T > &items )
	{
		std::vector< T > ret;
		ret.reserve( items.size() );
		for( typename std::set< T >::const_iterator i = items.begin(); i != items.end(); ++i )
			ret.push_back( *i );
		return ret;
	}

	/// Output a container to the stream.
	template< class Container >
		std::ostream &outputContainer( std::ostream &o, const Container &x, const char *delimiter = " " );

	/// Returns true if the vector contains the given 'val'. O(n).
	template< class T >
		inline bool inVector( const T& vec, const typename T::value_type& val )
	{
		return vec.end() != find( vec.begin(), vec.end(), val );
	}

	/// Returns true if the set contains the given 'val'. O(lg n).
	template< class T >
		inline bool inSet( const T& s, const typename T::value_type& val )
	{
		return s.end() != s.find( val );
	}

	/// Returns true if the map contains and entry with the given key. O(lg n).
	/// Should work for multi-maps as well.
	template< class T >
		inline bool inMap( const T& m, const typename T::key_type& key )
	{
		return m.end() != m.find( key );
	}

	typedef std::vector< float > FloatVec;
	typedef std::vector< float >::iterator IFloatVec;
	typedef std::vector< float >::const_iterator CIFloatVec;
	typedef std::vector< float >::reverse_iterator RIFloatVec;
	typedef std::vector< float >::const_reverse_iterator CRIFloatVec;

	typedef std::vector< double >::iterator IDoubleVec;
	typedef std::vector< double >::const_iterator CIDoubleVec;
	typedef std::vector< double >::reverse_iterator RIDoubleVec;
	typedef std::vector< double >::const_reverse_iterator CRIDoubleVec;

	typedef std::vector< DoubleVec >::iterator IDoubleVecVec;
	typedef std::vector< DoubleVec >::const_iterator CIDoubleVecVec;
	typedef std::vector< DoubleVec >::reverse_iterator RIDoubleVecVec;
	typedef std::vector< DoubleVec >::const_reverse_iterator CRIDoubleVecVec;

	typedef std::set< double > DoubleSet;
	typedef std::set< double >::iterator IDoubleSet;
	typedef std::set< double >::const_iterator CIDoubleSet;
	typedef std::set< double >::reverse_iterator RIDoubleSet;
	typedef std::set< double >::const_reverse_iterator CRIDoubleSet;

	typedef std::vector< bool > BoolVec;

	typedef std::vector< int > IntVec;
	typedef std::vector< int >::iterator IIntVec;
	typedef std::vector< int >::const_iterator CIIntVec;
	typedef std::vector< int >::reverse_iterator RIIntVec;
	typedef std::vector< int >::const_reverse_iterator CRIIntVec;

	typedef vector< vector< int > > IntVec2;
	typedef IntVec2::const_iterator CIIntVec2;
	typedef IntVec2::reverse_iterator RIIntVec2;
	typedef IntVec2::const_reverse_iterator CRIIntVec2;
	typedef vector< vector< vector< int > > > IntVec3;
	typedef vector< vector< vector< vector< int > > > > IntVec4;

	typedef std::vector< uint > UIntVec;
	typedef vector< vector< uint > > UIntVec2;
	typedef vector< vector< vector< uint > > > UIntVec3;
	typedef vector< vector< vector< vector< uint > > > > UIntVec4;

	typedef vector< vector< double > > DoubleVec2;
	typedef vector< vector< vector< double > > > DoubleVec3;
	typedef vector< vector< vector< vector< double > > > > DoubleVec4;

	typedef std::vector< IntVec > IntVecVec;
	typedef IntVecVec::iterator IIntVecVec;
	typedef IntVecVec::const_iterator CIIntVecVec;
	typedef IntVecVec::reverse_iterator RIIntVecVec;
	typedef IntVecVec::const_reverse_iterator CRIIntVecVec;

	typedef std::set< IntVec > IntVecSet;
	typedef IntVecSet::iterator IIntVecSet;
	typedef IntVecSet::const_iterator CIIntVecSet;
	typedef IntVecSet::reverse_iterator RIIntVecSet;
	typedef IntVecSet::const_reverse_iterator CRIIntVecSet;

	typedef std::set< int > IntSet;
	typedef std::set< int >::iterator IIntSet;
	typedef std::set< int >::const_iterator CIIntSet;
	typedef std::set< int >::reverse_iterator RIIntSet;
	typedef std::set< int >::const_reverse_iterator CRIIntSet;

	typedef std::map< int, int > IntIntMap;
	typedef std::map< int, int >::iterator IIntIntMap;
	typedef std::map< int, int >::const_iterator CIIntIntMap;
	typedef std::map< int, int >::reverse_iterator RIIntIntMap;
	typedef std::map< int, int >::const_reverse_iterator CRIIntIntMap;



	typedef std::vector< uint > UintVec;
	typedef std::vector< uint >::iterator IUintVec;
	typedef std::vector< uint >::const_iterator CIUintVec;
	typedef std::vector< uint >::reverse_iterator RIUintVec;
	typedef std::vector< uint >::const_reverse_iterator CRIUintVec;

	typedef std::set< uint > UintSet;
	typedef std::set< uint >::iterator IUintSet;
	typedef std::set< uint >::const_iterator CIUintSet;
	typedef std::set< uint >::reverse_iterator RIUintSet;
	typedef std::set< uint >::const_reverse_iterator CRIUintSet;

	typedef std::map< uint, uint > UintUintMap;
	typedef std::map< uint, uint >::iterator IUintUintMap;
	typedef std::map< uint, uint >::const_iterator CIUintUintMap;
	typedef std::map< uint, uint >::reverse_iterator RIUintUintMap;
	typedef std::map< uint, uint >::const_reverse_iterator CRIUintUintMap;

	// MIKE
	typedef set< unsigned int > UIntSet;
	inline ostream& operator<<( ostream& out, const UIntSet& s )
	{
		out << "[ ";
		for( auto& x : s ) out << x << " ";
		out << "]";
		return out;
	}

	typedef unordered_set< unsigned int > UIntUSet;
	inline ostream& operator<<( ostream& out, const UIntUSet& s )
	{
		out << "[ ";
		for( auto& x : s ) out << x << " ";
		out << "]";
		return out;
	}


	typedef std::vector< std::string > StringVec;
	typedef StringVec::iterator IStringVec;
	typedef StringVec::const_iterator CIStringVec;

	inline std::ostream &operator <<( std::ostream &o, const DoubleVec &x )    { o << "DoubleVec( ";    outputContainer( o, x ) << " )"; return o; }
	inline std::ostream &operator <<( std::ostream &o, const DoubleVecVec &x ) { o << "DoubleVecVec( "; outputContainer( o, x ) << " )"; return o; }
	inline std::ostream &operator <<( std::ostream &o, const IntVec &x )       { o << "IntVec( ";       outputContainer( o, x ) << " )"; return o; }
	inline std::ostream &operator <<( std::ostream &o, const IntVecVec &x )    { o << "IntVecVec( ";    outputContainer( o, x ) << " )"; return o; }
	inline std::ostream &operator <<( std::ostream &o, const UintVec &x )      { o << "UintVec( ";      outputContainer( o, x ) << " )"; return o; }
	inline std::ostream &operator <<( std::ostream &o, const StringVec &x )    { o << "StringVec( ";    outputContainer( o, x ) << " )"; return o; }
	inline std::ostream &operator <<( std::ostream &o, const IntSet &x )       { o << "IntSet( ";       outputContainer( o, x ) << " )"; return o; }
	inline std::ostream &operator <<( std::ostream &o, const FloatVec &x )     { o << "FloatVec( ";     outputContainer( o, x ) << " )"; return o; }
	inline std::ostream &operator <<( std::ostream &o, const Max6IntVec &x )   { o << "Int6Vec( ";     outputContainer( o, x ) << " )"; return o; }
	inline std::ostream &operator <<( std::ostream &o, const Max8IntVec &x )   { o << "Int8Vec( ";     outputContainer( o, x ) << " )"; return o; }
	inline std::ostream &operator <<( std::ostream &o, const Max12IntVec &x )  { o << "Int12Vec( ";     outputContainer( o, x ) << " )"; return o; }
	inline std::ostream &operator <<( std::ostream &o, const Max3IntVec &x )   { o << "CoordVec( ";     outputContainer( o, x ) << " )"; return o; }
	inline std::ostream &operator <<( std::ostream &o, const Max3IntVec2 &x )  { o << "CoordVecVec( ";  outputContainer( o, x ) << " )"; return o; }

	template< typename T1, typename T2 >
	inline std::ostream& operator <<( std::ostream& out, const std::pair< T1, T2 > &m )
	{
		out << "( first: " << m.first << ", second: " << m.second << " )";
		return out;
	}

	// make map output pastable into Julia
	template< typename T1, typename T2 >
	inline std::ostream& operator <<( std::ostream& out, const std::map< T1, T2 > &m )
	{
		out << "{ ";
		for( auto &i : m )
		{
			out << i.first << "=> ";
			out << i.second;
			out<< ", ";
		}
		out << " }";
		return out;
	}

	template< int A >
    	inline std::ostream &operator <<( std::ostream &o, const FastVector< double, A>  &x )  { o << "FastDoubleVec( ";  outputContainer( o, x ) << " )"; return o; }
	inline std::ostream &operator <<( std::ostream &o, const FastDoubleVec &x )
	{
		o << "FastDoubleVec " << x.size() << " ( ";
		for( int i = 0; i < x.size(); i++ )
		{
			if( i != 0 )
				o << " ";
			o << x[i];
		}
		cout << " )";
		return o;
	}


	inline std::ostream &lf( std::ostream &o, const DoubleVec &x )    { return o << "{ "; outputContainer( o, x, ", " ) << "}"; }


	/// Implementation of outputContainer after use in overloaded output container.
	template< class Container >
		std::ostream &outputContainer( std::ostream &o, const Container &x, const char *delimiter )
	{
		for( typename Container::const_iterator ic = x.begin(); ic != x.end(); ++ic )
		{
			if( ic != x.begin() )
				o << delimiter;
			o << *ic;
		}
		return o;
	}
	template< class Container >
		std::ostream &jsonOutputContainer( std::ostream &o, const Container &x )
	{
		o << "[ ";
		for( typename Container::const_iterator ic = x.begin(); ic != x.end(); ++ic )
		{
			if( ic != x.begin() )
				o << ", ";
			o << *ic;
		}
		o << " ]";
		return o;
	}

	template< class Container >
		std::ostream &outputBraceContainerWith( std::ostream &o, const Container &x, std::ostream& (outputer)( std::ostream &o, const typename Container::value_type &x),
												const char *left = "{ ",  const char *delimiter =", ", const char *right = " }" )
	{
		o << left;
		for( typename Container::const_iterator ic = x.begin(); ic != x.end(); ++ic )
		{
			if( ic != x.begin() )
				o << delimiter;
			outputer(o, *ic);
		}
		o << right;;
		return o;
	}

	template< class Container >
		std::ostream &outputBraceContainer( std::ostream &o, const Container &x,  const char *left = "{ ",  const char *delimiter =", ", const char *right = " }" )
	{
		o << left;
		for( typename Container::const_iterator ic = x.begin(); ic != x.end(); ++ic )
		{
			if( ic != x.begin() )
				o << delimiter;
			o << *ic;
		}
		o << right;;
		return o;
	}

	template<class Container, class T> bool contains( const Container& container, const T& value)
	{
		//return container.count( value ) > 0;
		typename Container::const_iterator iend = container.end();
		return std::find(container.begin(), iend, value) != iend;
	}

	template<typename Container, class T> bool doesnt_contain( const Container& container, const T& value)
	{
		//return container.count( value ) == 0;
		typename Container::const_iterator iend = container.end();
		return std::find(container.begin(), iend, value) == iend;
	}

	/// Convert a single index into the tensor product components
	template<typename VEC>
	inline void getTPIndices( uint index, VEC sizes, VEC &indices )
	{
		// get the tensor product components from the index.
		indices.resize( sizes.size(), 0 );
		for( uint idim = 0; idim < sizes.size(); ++idim )
		{
			uint current_order_size = sizes[ idim ];
			indices[ idim ] = index % current_order_size;
			index = index / current_order_size;
		}
	}


#ifdef PI
	const double pi = PI;
#else
	const double pi = 3.14159265358979323846;
#endif
	const double rad_to_deg = 180.0 / pi;
	const double deg_to_rad = pi / 180.0;

	inline bool isTTY() {
#ifndef _WIN32
		return isatty(fileno(stdin));
#else
		return _isatty(fileno(stdin));
#endif
	}
	inline bool stdoutIsTTY() {
#ifndef _WIN32
		return isatty(fileno(stdout));
#else
		return _isatty(fileno(stdout));
#endif
	}
	inline bool stderrIsTTY() {
#ifndef _WIN32
		return isatty(fileno(stderr));
#else
		return _isatty(fileno(stderr));
#endif
	}

	inline string currentTimeStampString() {
#ifdef _WIN32
#ifdef __MINGW32__
		char timestr[30];
		struct tm *dt;
		time_t t;
		time( &t );
		dt = localtime( &t );
		strftime(timestr, sizeof(timestr), "%Y-%m-%d_%H:%M:%S", dt);
		return string( timestr );
#else
		char timestr[30];
		struct tm dt; 
		__time64_t long_time;
		_time64( &long_time );
		_localtime64_s( &dt, &long_time );
		strftime(timestr, sizeof(timestr), "%Y-%m-%d_%H:%M:%S", &dt);
		return string( timestr );
#endif
#else
		struct tm dt;
		char timestr[30];
		struct timeval tv;

		gettimeofday(&tv, NULL);
		localtime_r(&tv.tv_sec, &dt);
		strftime(timestr, sizeof(timestr), "%Y-%m-%d_%H:%M:%S", &dt);
		return string( timestr );
#endif
	}

    inline void flatten_dvec2( const DoubleVec2 &input, DoubleVec &output )
    {
		output.clear();
		for( uint i = 0; i < input.size(); ++i )
			for( uint j = 0; j < input[ i ].size(); ++j )
				output.push_back( input[ i ][ j ] );
    }

    template< typename C >
    inline long sum_size_of( const C &v );

    template< typename T >
    inline long sum_size_of( const vector< vector< T > > &v  )
    {
	    long size = 0;
	    for( typename vector< vector< T > > ::const_iterator i = v.begin(); i != v.end(); ++i )
	    {
	    	    size += sum_size_of( *i );
	    }
	    return size + sizeof( vector< vector< T > > );
    }


    template< typename T >
    inline long size_of( const vector< T > &v  );
    template< typename C >
    inline long sum_size_of( const C &v );
    template< typename K >
    inline long sum_size_of( const map< K, DoubleVec > &v )
    {
	    long size = 0;
	    for( typename map< K, DoubleVec >::const_iterator i = v.begin(); i != v.end(); ++i )
	    {
		    size += i->first.size_of();
		    size += util::size_of( i->second );
	    }
	    return size + sizeof( v );
    }
    template< typename K, typename V >
    inline long sum_size_of_kV( const map< K, V > &v )
    {
	    long size = 0;
	    for( typename map< K, V >::const_iterator i = v.begin(); i != v.end(); ++i )
	    {
		    size += sizeof( i->first ) + i->first.length();
		    size += sum_size_of( i->second );
	    }
	    return size + sizeof( v );
    }
    template< typename K >
    inline long sum_size_of_K( const map< K, uint > &v )
    {
	    long size = 0;
	    for( typename map< K, uint >::const_iterator i = v.begin(); i != v.end(); ++i )
	    {
		    size += i->first.size_of();
		    size += sizeof( i->second );
	    }
	    return size + sizeof( v );
    }

    template< typename C >
    inline long sum_size_of( const C &v )
    {
	    long size = 0;
	    for( typename C::const_iterator i = v.begin(); i != v.end(); ++i )
	    {
		    size += i->size_of();
	    }
	    return size;
    }

    template< typename T >
    inline long size_of( const vector< vector< T > > &v  )
    {
	    long size = 0;
	    for( typename vector< vector< T > > ::const_iterator i = v.begin(); i != v.end(); ++i )
	    {
	    	    size += i->size() * sizeof( T ) + sizeof( vector< T > );
	    }
	    return size + sizeof( vector< vector< T > > );
    }
    template< typename T >
    inline long size_of( const vector< T > &v  )
    {
	    return v.size() * sizeof( T ) + sizeof( vector< T > );
    }
    template< typename T >
    inline long size_of( const set< T > &v  )
    {
	    return v.size() * sizeof( T ) + sizeof( set< T > );
    }
    template< typename T >
    inline long size_of( const unordered_set< T > &v  )
    {
	    return v.size() * sizeof( T ) + sizeof( unordered_set< T > );
    }
    template< typename T, typename T2 >
    inline long size_of( const unordered_set< T, T2 > &v  )
    {
	    return v.size() * sizeof( T ) + sizeof( unordered_set< T, T2 > );
    }
    template< typename T, typename V >
    inline long size_of( const unordered_map< T, V > &v  )
    {
	    return v.size() * sizeof( T ) + sizeof( unordered_map< T, V > );
    }
    template< typename T, typename V >
    inline long size_of( const map< T, V > &v  )
    {
	    return v.size() * sizeof( T ) + sizeof( map< T, V > );
    }

    long output_sizes( ostream& out, const string &aggregate, const StringVec &descs, long *s );
    string humanize_long( long x );


	//FIXME more efficient to use an expression AST
	inline CoordVec add( const CoordVec& v1, const CoordVec& v2 )
	{
		const uint dimN = v1.size();
		CoordVec r;
		for( uint idim = 0; idim < dimN; ++idim )
		{
			r.push_back( v1[ idim ] + v2[ idim ]  );
		}
		return r;
	}

	inline int flatten( const CoordVec x, const CoordVec& max )
	{
		int index = x[0];
		int mult = 1;
		for( int i = 1; i < x.size(); ++i )
		{
			mult *= max[ i - 1 ];
			index += x[i] * mult;
		}
		return index;
	}

	inline CoordVec unflatten( const uint index, const CoordVec& max )
	{
		CoordVec coord;
		getTPIndices( index, max, coord );
		return coord;
	}


	/// A visited class that tracks which items have been visited before
	template< typename T >
	class Visited
	{
		public:
		bool operator() ( const T& t )
		{
			pair< typename SetType::const_iterator, bool > result = mSet.insert( t );
			return ! result.second;
		}
		bool contains( const T& t ) const { return mSet.find( t ) != mSet.end(); }
		size_t size() const { return mSet.size(); }
		protected:
		typedef unordered_set< T > SetType;
		unordered_set< T > mSet;
	};

	inline ostream& outputVec( ostream& out, const VectorXd& v )
	{
		out << "[ ";
		for( int i = 0; i < v.size(); ++i  ) out << v[ i ] << " ";
		out << "]";
		return out;
	}

	inline DoubleVec toDoubleVec( const VectorXd& v )
	{
		DoubleVec ret;
		for( uint i = 0; i < v.size(); ++i )
			ret.push_back( v( i ) );
		return ret;
	}

	typedef Matrix< double, 6, 1 > Vector6d;
	typedef vector< Vector6d > Vector6dVec;

	typedef Matrix< double, 6, 3 > Matrix6_3d;
	typedef vector< Matrix6_3d > Matrix6_3dVec;

	typedef Matrix< double, 3, 6 > Matrix3_6d;
	typedef vector< Matrix3_6d > Matrix3_6dVec;

	typedef Matrix< double, 6, 6 > Matrix6_6d;
	typedef vector< Matrix6_6d > Matrix6_6dVec;

	typedef Matrix< double, 6, 8 > Matrix6_8d;

	typedef Matrix< double, 8, 6 > Matrix8_6d;
	typedef vector< Matrix8_6d > Matrix8_6dVec;

	typedef Matrix< double, 8, 8 > Matrix8_8d;

	typedef Matrix< double, 2, 6 > Matrix2_6d;
	typedef vector< Matrix2_6d > Matrix2_6dVec;

	typedef Matrix< double, 1, 6 > Matrix1_6d;
	typedef vector< Matrix1_6d > Matrix1_6dVec;

	typedef vector< VectorXd > VectorXdVec;

	typedef Matrix< double, 1, 1 > Vector1d;

	typedef Matrix< double, 3, 2 > Matrix3_2d;
	typedef Matrix< double, 2, 3 > Matrix2_3d;
	typedef vector< Matrix2_3d > Matrix2_3dVec;
	typedef Matrix< double, 3, 1 > Matrix3_1d;
	typedef Matrix< double, 1, 1 > Matrix1d;

	typedef Matrix< double, 8, 1 > Vector8d;

	typedef vector< Matrix3d > Matrix3dVec;




	inline string fileExtension( string filename )
	{
		size_t pos = filename.find_last_of('.');
		if( pos < filename.length() )
		{
			return filename.substr( pos + 1 );
		}
		else
		{
			return "";
		}
	}

	inline bool hasExtension( string filename, const string& ext )
	{
		size_t pos = filename.find_last_of('.');
		if( pos < filename.length() )
		{
			return filename.substr( pos + 1 ) == ext;
		}
		else
		{
			return false;
		}
	}
	
	//replace extension on filename
	/// i.e. replaceExtension( "foo.bar.buzz", "loo" ) -> "foo.bar.loo"
	inline string replaceExtension( string filename, const string& ext )
	{
		size_t pos = filename.find_last_of('.');
		if( pos < filename.length() )
		{
			filename.resize( pos + 1 );
		}
		else
		{
			filename += '.';
		}
		return filename + ext;
	}

	//replace extension on filename
	/// i.e. replaceExtension( "foo.bar.buzz", ".loo" ) -> "foo.bar.loo"
	inline string replaceExtensionIncludingPeriod( string filename, const string& ext )
	{
		size_t pos = filename.find_last_of('.');
		if( pos < filename.length() )
		{
			filename.resize( pos );
		}
		return filename + ext;
	}

	///inserts to_insert before extension.
	inline string insertBeforeExtension( string filename, const string& to_insert )
	{
		size_t pos = filename.find_last_of('.');
		if( pos < filename.length() )
		{
			string ext = filename.substr( pos );
			filename.resize( pos );
			return filename + to_insert + ext;
		}
		return filename + to_insert;
	}

	template< typename T >
	class Maybe
	{
		protected:
		T mT;
		const bool mValid;

		public:
		Maybe() : mValid( false ) {}
		Maybe( const T& t ) : mT( t ), mValid( true ) {}

		bool operator() () const{ return mValid; }
		bool isValid() const { return mValid; }
		T val()
		{
			if ( mValid ) return mT;
			else COUT_THROW_EXCEPTION_AT( IGXException, "Called maybe on a nothing type" );
		}

		template< class U = T >
		typename enable_if< ! is_const<U>::value, const U>::type val() const
		{
			if ( mValid ) return mT;
			else COUT_THROW_EXCEPTION_AT( IGXException, "Called maybe on a nothing type" );
		}
	};

	template< class X > Maybe< X > Just( const X& x ) { return Maybe< X >( x ); }

	template< class X > Maybe< X > Nothing() { return Maybe< X >(); }

        template< typename T >
	inline ostream& operator<<( ostream& out, const Maybe< T >& m )
	{
		if( m.isValid() )
			out << "Just( " << m.val() << " )";
		else
			out << "Nothing" << endl;
		return out;
	}

	template< typename T >
	class MaybeRef
	{
		protected:
		T* mT;
		const bool mValid;

		public:
		MaybeRef() : mValid( false ) {}
		MaybeRef( T& t ) : mT( &t ), mValid( true ) {}

		bool operator() () const { return mValid; }
		bool isValid() const { return mValid; }
		T& val()
		{
			if ( mValid ) return *mT;
			else COUT_THROW_EXCEPTION_AT( IGXException, "Called maybe on a nothing type" );
		}

		typename enable_if< ! is_const<T&>::value, const T&>::type val() const
		{

			if ( mValid ) return *mT;
			else COUT_THROW_EXCEPTION_AT( IGXException, "Called maybe on a nothing type" );
		}
	};

	template< class X > MaybeRef< X > JustRef( X& x ) {return MaybeRef< X >( x ); }

	template< class X > MaybeRef< X > NothingRef() { return MaybeRef< X >(); }

	template< class X > void onJust( Maybe< X > mb, std::function< void ( X x ) > callback ) { 
		if( mb.isValid() )
		{
			callback( mb.val() );
		}
	}

	template< class X > void onNothing( Maybe< X > mb, std::function< void () > callback ) { 
		if( not mb.isValid() )
		{
			callback();
		}
	}

	template< class X > void onMaybe( Maybe< X > mb, std::function< void ( X x ) > just_callback , std::function< void () > nothing_callback ) { 
		if( mb.isValid() )
		{
			just_callback( mb.val() );
		}
		else
		{
			nothing_callback();
		}
	}
	template< class X > void onJust( MaybeRef< X > mb, std::function< void ( X& x ) > callback ) { 
		if( mb.isValid() )
		{
			callback( mb.val() );
		}
	}

	template< class X > void onNothing( MaybeRef< X > mb, std::function< void () > callback ) { 
		if( not mb.isValid() )
		{
			callback();
		}
	}

	template< class X > void onMaybe( MaybeRef< X > mb, std::function< void ( X& x ) > just_callback , std::function< void () > nothing_callback ) { 
		if( mb.isValid() )
		{
			just_callback( mb.val() );
		}
		else
		{
			nothing_callback();
		}
	}

	typedef Matrix< double, 1, 1 > Vector1d;

	template< typename T >
	inline ostream& operator<<( ostream& out, const set< T >& scm )
	{
		out << "{ ";
		for( auto &x : scm )
		{
			out << x << " ";
		}
		out << "}";
		return out;
	}
        template< typename T >
	inline ostream& operator<<( ostream& out, const unordered_set< T >& scm )
	{
		out << "u{ ";
		for( auto &x : scm )
		{
			out << x << " ";
		}
		out << "}";
		return out;
	}
        template< typename T >
	inline ostream& operator<<( ostream& out, const vector< T >& scm )
	{
		out << "[ ";
		for( auto &x : scm )
		{
			out << x << " ";
		}
		out << "]";
		return out;
	}

	inline string slurp(ifstream& in) {
		return static_cast<stringstream const&>(stringstream() << in.rdbuf()).str();
	}
	inline string slurp( const string& filename ) {
		ifstream ifs( filename );
		if( ifs.fail() )
			COUT_THROW_EXCEPTION_AT( IGXException, "Can't open file: " << filename << endl );
		return slurp( ifs );
	}
}
#endif
