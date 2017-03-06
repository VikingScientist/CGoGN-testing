#pragma once

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

namespace util
{

	using namespace std;

	template< class T >
		class ArrayIterator
	{
		T *mPtr;
		
		public:
		typedef bidirectional_iterator_tag iterator_category;
		typedef T value_type;
		typedef T& reference;
		typedef T& T_REFERENCE;
		typedef T* T_POINTER;
		typedef T* pointer;
		typedef int DIFFERENCE_TYPE;
		typedef int difference_type;

		ArrayIterator() : mPtr( NULL ) {}
		ArrayIterator( T *ptr) : mPtr( ptr ) {}
		T_REFERENCE operator*() const { return *mPtr; }
		ArrayIterator& operator++() { ++mPtr; return (*this); } //preincrement
		ArrayIterator& operator--() { --mPtr; return (*this); }
		ArrayIterator& operator++(int) { ArrayIterator orig( mPtr ); ++(*this); return orig; } //postincrement
		ArrayIterator& operator--(int) { ArrayIterator orig( mPtr ); --(*this); return orig; } //postdecrement
		bool operator==( const ArrayIterator &right ) const { return mPtr == right.mPtr; }
		bool operator!=( const ArrayIterator &right ) const { return !(mPtr == right.mPtr); }
	};

#ifndef NDEBUG
#define FASTVECTOR_DEBUG
#endif

	template< class T, int MAXSIZE >
		class FastVector
	{
		public:

		typedef ArrayIterator< T > iterator;
		typedef ArrayIterator< const T > const_iterator;

		FastVector() : mLength( 0 )
		{
		}

		FastVector( const vector< T > &v ) : mLength( 0 )
		{
#ifdef FASTVECTOR_DEBUG
			if( v.size() > mMaxLength )
				exit_message( string( "FastVector::FastVector( const vector< T > &v ) overflow " ) + to_string( v.size() ) );
#endif
				for( auto& x : v )
					push_back( x );
		}

/*
		FastVector( const_iterator st, const_iterator stop )
		{
#ifdef FASTVECTOR_DEBUG
			if( ( stop - st ) > mMaxLength )
				exit_message( string( "FastVector::FastVector(const T* vals, const int size) overflow " ) + to_string( size ) );
#endif
				std::copy( begin(), st, stop );
		}
*/
		FastVector( int size ) : mLength( size )
		{
#ifdef FASTVECTOR_DEBUG
			if( size > mMaxLength )
				exit_message( string( "FastVector::FastVector(const T* vals, const int size) overflow " ) + to_string( size ) );
#endif
		}

		FastVector( int size, const T& val ) : mLength( 0 )
		{
#ifdef FASTVECTOR_DEBUG
			if( size > mMaxLength )
				exit_message( string( "FastVector::FastVector(const T* vals, const int size) overflow " ) + to_string( size ) );
#endif
			for( int i = 0; i < size; ++i ) { mVals[i] = val; }
			mLength = size;
		}

		FastVector( std::initializer_list<T> c )
		{
#ifdef FASTVECTOR_DEBUG
			if( c.size() > mMaxLength )
				exit_message( string( "FastVector::FastVector(const T* vals, const int size) overflow " ) + to_string( c.size() ) );
#endif
			//memcpy( mVals, c.begin(), sizeof(T) * c.size());
			std::copy( c.begin(), c.end(), mVals );
			//cout << "HERE" << mVals[0] << " " << *c.begin() << " " << c.size() << endl;
			mLength = c.size();
		}


		FastVector(const T* vals, const int size)
		{
#ifdef FASTVECTOR_DEBUG
			if( size > mMaxLength )
				exit_message( string( "FastVector::FastVector(const T* vals, const int size) overflow " ) + to_string( size ) );
#endif
			memcpy( mVals, vals, sizeof(T) * size);
			mLength = size;
		}

		FastVector &operator= ( const vector< T >& vals )
		{
#ifdef FASTVECTOR_DEBUG
				if( vals.size() > mMaxLength )
					exit_message( string( "FastVector::operator= ( const vector< T >V& vals ) overflow " ) + to_string( vals.size() ) );
#endif
				mLength = vals.size();
				memcpy( mVals, vals.data(), sizeof(T) * mLength);
				return *this;
			}
		FastVector &operator= ( std::initializer_list<T> c )
		{
#ifdef FASTVECTOR_DEBUG
			if( c.size() > mMaxLength )
				exit_message( string( "FastVector::FastVector(const T* vals, const int size) overflow " ) + to_string( c.size() ) );
#endif
			//memcpy( mVals, c.begin(), sizeof(T) * c.size());
			std::copy( c.begin(), c.end(), mVals );
			//cout << "HERE" << mVals[0] << " " << *c.begin() << " " << c.size() << endl;
			mLength = c.size();
			return *this;
		}


		void push_back( const T v )
		{
#ifdef FASTVECTOR_DEBUG
			if( mLength >= mMaxLength )
				exit_message( string( "FastVector::push_back( const T v ) overflow " ) + to_string( mLength ) );
#endif
			mVals[mLength] = v;
			mLength++;
		}

		T &operator[] ( const int pos )
		{
#ifdef FASTVECTOR_DEBUG
			if( pos >= mLength )
				exit_message( string( "FastVector::operator[] overflow " ) + to_string( pos ) );
#endif
			return mVals[ pos ];
		}

		const T &operator[] ( const int pos ) const
		{
#ifdef FASTVECTOR_DEBUG
			if( pos >= mLength )
				exit_message( string( "FastVector::operator[] const overflow " ) + to_string( pos ) );
#endif
			return mVals[ pos ];
		}

		bool operator<( const FastVector &o ) const
		{
			if( size() != o.size() ) return false;
			for( int i = 0; i < mLength; ++i )
			{
				const int ind = mLength - i - 1;
				if( mVals[ind] > o.mVals[ind] ) return false;
				if( mVals[ind] < o.mVals[ind] ) return true;
			}
			return false;
		}

		bool operator==( const FastVector &o ) const
		{
			if( size() != o.size() ) return false;
			for( int i = 0; i < mLength; ++i )
			{
				if( mVals[i] != o.mVals[i] ) return false;
			}
			return true;
		}

		bool operator!=( const FastVector &o ) const
		{
			return ! operator==( o );
		}


		int size() const 
		{
			return mLength;
		}

		void clear()
		{
			mLength = 0;
		}

		void resize(const int size)
		{
#ifdef FASTVECTOR_DEBUG
			if( size > mMaxLength ) 
				exit_message( string( "FastVector::resize overflow " ) + to_string( size ) );
#endif
			mLength = size;
		}
		void resize(const int size, const T val)
		{
#ifdef FASTVECTOR_DEBUG
			if( size > mMaxLength ) 
				exit_message( string( "FastVector::resize overflow " ) + to_string( size ) );
#endif
			if( size <= mLength )
				mLength = size;
			else
			{
				std::fill( mVals + mLength, mVals + mLength + (size - mLength), val );
				mLength = size;
			}
		}

		T* data() { return mVals; }
		const T* data() const { return mVals; }

		iterator begin() { return iterator( mVals ); }
		iterator end()   { return iterator( mVals + mLength ); }

		const_iterator begin() const { return const_iterator( mVals ); } 
		const_iterator end()   const { return const_iterator( mVals + mLength ); }

		static const int mMaxLength = MAXSIZE;
		IGA_ALIGN_16 T mVals[ mMaxLength ];
		int mLength;
	};

	typedef FastVector< int, 3 > CoordVec;
	typedef FastVector< int, 3 > DimVec;
	typedef FastVector< int, 3 > DegVec;
	typedef FastVector< double, 15 > FastDoubleVec;
	typedef FastVector< double, 3 > Max3DoubleVec;
	typedef FastVector< int, 3 > Max3IntVec;
	typedef vector< FastVector< int, 3 > > Max3IntVec2;
	typedef FastVector< bool, 6 >  Max6BoolVec;
	typedef FastVector< bool, 8 >  Max8BoolVec;
	typedef FastVector< bool, 12 >  Max12BoolVec;
	typedef FastVector< int, 6 >  Max6IntVec;
	typedef FastVector< int, 8 >  Max8IntVec;
	typedef FastVector< int, 12 >  Max12IntVec;

	template< typename T, int A >
        inline bool equals( const FastVector< T, A > &v1, const FastVector< T, A > &v2, double tol = 1e-8 )
	{
		if( v1.size() != v2.size() ) return false;
		for( uint i=0; i < v1.size(); i++ )
		{
			if ( !util::equals(v1[i], v2[i], tol) )
				return false;
		}
		return true;
	}

	template< int B >
	inline bool equals( const FastVector< int, B > &v1, const vector< int > &v2 )
	{
		if( v1.size() != v2.size() ) return false;
		for( uint i=0; i < v1.size(); i++ )
		{
			if ( v1[i] != v2[i] )
				return false;
		}
		return true;
	}

	template< int B >
	inline bool equals( const vector< int > &v1, const FastVector< int, B > &v2 )
	{
		for( uint i=0; i < v1.size(); i++ )
		{
			if ( v1[i] != v2[i] )
				return false;
		}
		return true;
	}

}
