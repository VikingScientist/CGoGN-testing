#include <Eigen/Core>
#include "common_util.h"
//#include "Point.h"

#include "sstream"
#include "iomanip"
#include <algorithm>

using namespace std;

namespace util
{
	// template<>
	// const Point< 2, int, false > Point< 2, int, false >::null = Point< 2, int, false >( 0, 0 );

	// template<>
	// const Point< 3, int, false > Point< 3, int, false >::null = Point< 3, int, false >( 0, 0, 0 );

	// template<>
	// const Point< 2, int, true > Point< 2, int, true >::null = Point< 2, int, true >( 0, 0, 0 );

	// template<>
	// const Point< 3, int, true > Point< 3, int, true >::null = Point< 3, int, true >( 0, 0, 0, 0 );

	// template<>
	// const Point< 2, float, false > Point< 2, float, false >::null = Point< 2, float, false >( 0.0f, 0.0f );

	// template<>
	// const Point< 3, float, false > Point< 3, float, false >::null = Point< 3, float, false >( 0.0f, 0.0f, 0.0f );

	// template<>
	// const Point< 2, float, true > Point< 2, float, true >::null = Point< 2, float, true >( 0.0f, 0.0f, 0.0f );

	// template<>
	// const Point< 3, float, true > Point< 3, float, true >::null = Point< 3, float, true >( 0.0f, 0.0f, 0.0f, 0.0f );

	// template<>
	// const Point< 2, double, false > Point< 2, double, false >::null = Point< 2, double, false >( 0.0, 0.0 );

	// template<>
	// const Point< 3, double, false > Point< 3, double, false >::null = Point< 3, double, false >( 0.0, 0.0, 0.0 );

	// template<>
	// const Point< 2, double, true > Point< 2, double, true >::null = Point< 2, double, true >( 0.0, 0.0, 0.0 );

	// template<>
	// const Point< 3, double, true > Point< 3, double, true >::null = Point< 3, double, true >( 0.0, 0.0, 0.0, 0.0 );

	// template< uint S, typename T >
	// const Point< S, T, false > Point< S, T, false >::null = Point< S, T, false >( NULL_VECTOR );

	// template< uint S, typename T >
	// const Point< S, T, true > Point< S, T, true >::null = Point< S, T, true >( NULL_VECTOR );

	// ostream &operator <<( ostream &o, const Space &x )
	// {
	// 	switch( x )
	// 	{
	// 		case H1: o << "H1"; return o;
	// 		case HCURL: o << "HCURL"; return o;
	// 		case HDIV: o << "HDIV"; return o;
	// 		case L2: o << "L2"; return o;
	// 		default: break;
	// 	}
	// 	o << (uint) x;
	// 	return o;
	// }
	
	// std::ostream &operator <<( std::ostream &o, const Degree &x )
	// {
	// 	o << "Degree( [";
	// 	for( uint idim = 0; idim < x.dim(); ++idim )
	// 		o << " " << x.deg( idim );
	// 	o << " )";
	// 	return o;
	// }

	// std::ostream &operator <<( std::ostream &o, const Axis &a )
	// {
	// 	switch( a )
	// 	{
	// 		case X: o << "X"; return o;
	// 		case Y: o << "Y"; return o;
	// 		case Z: o << "Z"; return o;
	// 		case W: o << "W"; return o;
	// 		default: break;
	// 	}
	// 	o << (uint) a;
	// 	return o;
	// }

	// std::istream &operator >>( std::istream &i, Axis &a )
	// {
	// 	while( isspace(i.peek()) ) i.ignore();
	// 	char next = i.peek();
	// 	if( next >= '0' && next <= '9' )
	// 	{
	// 		int val;
	// 		i >> val;
	// 		a = (Axis) val;
	// 		return i;
	// 	}
	// 	switch( next )
	// 	{
	// 	case 'X': a = X; break;
	// 	case 'Y': a = Y; break;
	// 	case 'Z': a = Z; break;
	// 	case 'W': a = W; break;
	// 	}
	// 	while( i && !isspace(i.peek()) ) i.ignore();
	// 	return i;
	// }

	// std::ostream &operator <<( std::ostream &o, const Side &x )
	// {
	// 	switch( x )
	// 	{
	// 	case BOTTOM : o << "BOTTOM"; return o;
	// 	case RIGHT	: o << "RIGHT"; return o;
	// 	case TOP	: o << "TOP"; return o;
	// 	case LEFT	: o << "LEFT"; return o;
	// 	case BACK	: o << "BACK"; return o;
	// 	case FORWARD: o << "FORWARD"; return o;
	// 	default		: break;
	// 	}
	// 	o << (uint) x;
	// 	return o;
	// }

	// std::istream &operator >>( std::istream &i, Side &x )
	// {
	// 	while( isspace(i.peek()) ) i.ignore();
	// 	char next = i.peek();
	// 	if( next >= '0' && next <= '9' )
	// 	{
	// 		int val;
	// 		i >> val;
	// 		x = (Side) val;
	// 		return i;
	// 	}
	// 	else
	// 	{
	// 		switch( next )
	// 		{
	// 		case 'B':
	// 			i.ignore();
	// 			next = i.peek();
	// 			if( next == 'O' )
	// 				x = BOTTOM;
	// 			else if( next == 'A' )
	// 				x = BACK;
	// 			break;
	// 		case 'R': x = RIGHT; break;
	// 		case 'T': x = TOP; break;
	// 		case 'L': x = LEFT; break;
	// 		case 'F': x = FORWARD; break;
	// 		}
	// 		// Eat the input.
	// 		while( i && !isspace(i.peek()) ) i.ignore();
	// 		return i;
	// 	}
	// }

	string humanize_long( long x )
	{
		stringstream ss;
		if( x > 1e15 )
		{
			ss << setprecision(5) <<  ( x / (double) 1e15  ) << " PB";
		}
		else if( x > 1e12 )
		{
			ss << setprecision(5) <<  ( x / (double) 1e12  ) << " TB";
		}
		else if( x > 1e9 )
		{
			ss << setprecision(5) <<  ( x / (double) 1e9  ) << " GB";
		}
		else if( x > 1e6 )
		{
			ss << setprecision(5) <<  ( x / (double) 1e6  ) << " MB";
		}
		else if( x > 1e3 )
		{
			ss << setprecision(5) <<  ( x / (double) 1e3  ) << " KB";
		}
		else
		{
			ss << setprecision(5) <<  x  << " B";
		}
		return ss.str();
	}

	long output_sizes( ostream& out, const string &aggregate, const StringVec &descs, long *s )
	{
		cout << aggregate << ":" << endl;
		unsigned long max_descs_len = 25;
		for( int i = 0; i < descs.size(); ++i )
		{
			max_descs_len = std::max( (size_t) max_descs_len, (size_t) descs[i].length() );
		}

		long size = 0;
		for( int i = 0; i < descs.size(); ++i )
		{
			out << " " << descs[i] << ": " << string( max_descs_len - descs[i].length(), ' ') << setw(12) << setfill(' ') << s[i] << " " << humanize_long( s[i] ) << endl;
			size += s[i];
		}
		out << " " << "Total" << ": " << string( max_descs_len - 5, ' ') << setw(12) << setfill(' ') << size << " " << humanize_long( size ) << endl;
		return size;
	}


}
