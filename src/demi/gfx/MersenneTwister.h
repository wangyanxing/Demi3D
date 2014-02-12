/**********************************************************************
This source file is a part of Demi3D
   __  ___  __  __  __
  |  \|_ |\/||   _)|  \ 
  |__/|__|  ||  __)|__/ 

Copyright (c) 2013-2014 Demi team
https://github.com/wangyanxing/Demi3D

Released under the MIT License
https://github.com/wangyanxing/Demi3D/blob/master/License.txt
***********************************************************************/

#ifndef DiMersenneTwister_h__
#define DiMersenneTwister_h__


#include <iostream>
#include <climits>
#include <cstdio>
#include <ctime>
#include <cmath>

namespace Demi
{
    class DiRand 
    {
        // Data
    public:
        typedef unsigned long uint32;  // unsigned integer type, at least 32 bits

        enum { N = 624 };       // length of state vector
        enum { SAVE = N + 1 };  // length of array for save()

    protected:
        enum { M = 397 };  // period parameter

        uint32 state[N];   // internal state
        uint32 *pNext;     // next value to get from state
        int left;          // number of values left before reload needed

        // Methods
    public:
        DiRand( const uint32 oneSeed );  // initialize with a simple uint32
        DiRand( uint32 *const bigSeed, uint32 const seedLength = N );  // or array
        DiRand();  // auto-initialize with /dev/urandom or time() and clock()
        DiRand( const DiRand& o );  // copy

        // Do NOT use for CRYPTOGRAPHY without securely hashing several returned
        // values together, otherwise the generator state can be learned after
        // reading 624 consecutive values.

        // Access to 32-bit random numbers
        uint32 RandInt();                     // integer in [0,2^32-1]
        uint32 RandInt( const uint32 n );     // integer in [0,n] for n < 2^32
        double Rand();                        // real number in [0,1]
        double Rand( const double n );        // real number in [0,n]
        double RandExc();                     // real number in [0,1)
        double RandExc( const double n );     // real number in [0,n)
        double RandDblExc();                  // real number in (0,1)
        double RandDblExc( const double n );  // real number in (0,n)
        double operator()();                  // same as rand()

        // Access to 53-bit random numbers (capacity of IEEE double precision)
        double Rand53();  // real number in [0,1)

        // Access to nonuniform random number distributions
        double RandNorm( const double mean = 0.0, const double stddev = 1.0 );

        // Re-seeding functions with same behavior as initializers
        void Seed( const uint32 oneSeed );
        void Seed( uint32 *const bigSeed, const uint32 seedLength = N );
        void Seed();

        // Saving and loading generator state
        void Save( uint32* saveArray ) const;  // to array of size SAVE
        void Load( uint32 *const loadArray );  // from such array
        friend std::ostream& operator<<( std::ostream& os, const DiRand& mtrand );
        friend std::istream& operator>>( std::istream& is, DiRand& mtrand );
        DiRand& operator=( const DiRand& o );

    protected:
        
        void            Init( const uint32 oneSeed );
        
        void            Reload();
        
        uint32          HiBit( const uint32 u ) const { return u & 0x80000000UL; }
        
        uint32          LoBit( const uint32 u ) const { return u & 0x00000001UL; }
        
        uint32          LoBits( const uint32 u ) const { return u & 0x7fffffffUL; }
        
        uint32          MixBits( const uint32 u, const uint32 v ) const
        { return HiBit(u) | LoBits(v); }

        uint32          Magic( const uint32 u ) const
        { return LoBit(u) ? 0x9908b0dfUL : 0x0UL; }

        uint32          Twist( const uint32 m, const uint32 s0, const uint32 s1 ) const
        { return m ^ (MixBits(s0,s1)>>1) ^ Magic(s1); }
        
        static uint32   Hash( time_t t, clock_t c );
    };

    // Functions are defined in order of usage to assist inlining

    inline DiRand::uint32 DiRand::Hash( time_t t, clock_t c )
    {
        // Get a uint32 from t and c
        // Better than uint32(x) in case x is floating point in [0,1]
        // Based on code by Lawrence Kirby (fred@genesis.demon.co.uk)

        static uint32 differ = 0;  // guarantee time-based seeds will change

        uint32 h1 = 0;
        unsigned char *p = (unsigned char *) &t;
        for( size_t i = 0; i < sizeof(t); ++i )
        {
            h1 *= UCHAR_MAX + 2U;
            h1 += p[i];
        }
        uint32 h2 = 0;
        p = (unsigned char *) &c;
        for( size_t j = 0; j < sizeof(c); ++j )
        {
            h2 *= UCHAR_MAX + 2U;
            h2 += p[j];
        }
        return ( h1 + differ++ ) ^ h2;
    }

    inline void DiRand::Init( const uint32 seed )
    {
        // Init generator state with seed
        // See Knuth TAOCP Vol 2, 3rd Ed, p.106 for multiplier.
        // In previous versions, most significant bits (MSBs) of the seed affect
        // only MSBs of the state array.  Modified 9 Jan 2002 by Makoto Matsumoto.
        register uint32 *s = state;
        register uint32 *r = state;
        register int i = 1;
        *s++ = seed & 0xffffffffUL;
        for( ; i < N; ++i )
        {
            *s++ = ( 1812433253UL * ( *r ^ (*r >> 30) ) + i ) & 0xffffffffUL;
            r++;
        }
    }

    inline void DiRand::Reload()
    {
        // Generate N new values in state
        // Made clearer and faster by Matthew Bellew (matthew.bellew@home.com)
        static const int MmN = int(M) - int(N);  // in case enums are unsigned
        register uint32 *p = state;
        register int i;
        for( i = N - M; i--; ++p )
            *p = Twist( p[M], p[0], p[1] );
        for( i = M; --i; ++p )
            *p = Twist( p[MmN], p[0], p[1] );
        *p = Twist( p[MmN], p[0], state[0] );

        left = N, pNext = state;
    }

    inline void DiRand::Seed( const uint32 oneSeed )
    {
        // Seed the generator with a simple uint32
        Init(oneSeed);
        Reload();
    }

    inline void DiRand::Seed( uint32 *const bigSeed, const uint32 seedLength )
    {
        // Seed the generator with an array of uint32's
        // There are 2^19937-1 possible initial states.  This function allows
        // all of those to be accessed by providing at least 19937 bits (with a
        // default seed length of N = 624 uint32's).  Any bits above the lower 32
        // in each element are discarded.
        // Just call seed() if you want to get array from /dev/urandom
        Init(19650218UL);
        register int i = 1;
        register uint32 j = 0;
        register int k = ( N > seedLength ? N : seedLength );
        for( ; k; --k )
        {
            state[i] =
                state[i] ^ ( (state[i-1] ^ (state[i-1] >> 30)) * 1664525UL );
            state[i] += ( bigSeed[j] & 0xffffffffUL ) + j;
            state[i] &= 0xffffffffUL;
            ++i;  ++j;
            if( i >= N ) { state[0] = state[N-1];  i = 1; }
            if( j >= seedLength ) j = 0;
        }
        for( k = N - 1; k; --k )
        {
            state[i] =
                state[i] ^ ( (state[i-1] ^ (state[i-1] >> 30)) * 1566083941UL );
            state[i] -= i;
            state[i] &= 0xffffffffUL;
            ++i;
            if( i >= N ) { state[0] = state[N-1];  i = 1; }
        }
        state[0] = 0x80000000UL;  // MSB is 1, assuring non-zero initial array
        Reload();
    }

    inline void DiRand::Seed()
    {
        // Seed the generator with an array from /dev/urandom if available
        // Otherwise use a hash of time() and clock() values

        // First try getting an array from /dev/urandom
        FILE* urandom = fopen( "/dev/urandom", "rb" );
        if( urandom )
        {
            uint32 bigSeed[N];
            register uint32 *s = bigSeed;
            register int i = N;
            register bool success = true;
            while( success && i-- )
                success = fread( s++, sizeof(uint32), 1, urandom ) != 0;
            fclose(urandom);
            if( success ) { Seed( bigSeed, N );  return; }
        }

        // Was not successful, so use time() and clock() instead
        Seed( Hash( time(NULL), clock() ) );
    }

    inline DiRand::DiRand( const uint32 oneSeed )
    { Seed(oneSeed); }

    inline DiRand::DiRand( uint32 *const bigSeed, const uint32 seedLength )
    { Seed(bigSeed,seedLength); }

    inline DiRand::DiRand()
    { Seed(); }

    inline DiRand::DiRand( const DiRand& o )
    {
        register const uint32 *t = o.state;
        register uint32 *s = state;
        register int i = N;
        for( ; i--; *s++ = *t++ ) {}
        left = o.left;
        pNext = &state[N-left];
    }

    inline DiRand::uint32 DiRand::RandInt()
    {
        // Pull a 32-bit integer from the generator state
        // Every other access function simply transforms the numbers extracted here

        if( left == 0 ) Reload();
        --left;

        register uint32 s1;
        s1 = *pNext++;
        s1 ^= (s1 >> 11);
        s1 ^= (s1 <<  7) & 0x9d2c5680UL;
        s1 ^= (s1 << 15) & 0xefc60000UL;
        return ( s1 ^ (s1 >> 18) );
    }

    inline DiRand::uint32 DiRand::RandInt( const uint32 n )
    {
        // Find which bits are used in n
        // Optimized by Magnus Jonsson (magnus@smartelectronix.com)
        uint32 used = n;
        used |= used >> 1;
        used |= used >> 2;
        used |= used >> 4;
        used |= used >> 8;
        used |= used >> 16;

        // Draw numbers until one is found in [0,n]
        uint32 i;
        do
        i = RandInt() & used;  // toss unused bits to shorten search
        while( i > n );
        return i;
    }

    inline double DiRand::Rand()
    { return double(RandInt()) * (1.0/4294967295.0); }

    inline double DiRand::Rand( const double n )
    { return Rand() * n; }

    inline double DiRand::RandExc()
    { return double(RandInt()) * (1.0/4294967296.0); }

    inline double DiRand::RandExc( const double n )
    { return RandExc() * n; }

    inline double DiRand::RandDblExc()
    { return ( double(RandInt()) + 0.5 ) * (1.0/4294967296.0); }

    inline double DiRand::RandDblExc( const double n )
    { return RandDblExc() * n; }

    inline double DiRand::Rand53()
    {
        uint32 a = RandInt() >> 5, b = RandInt() >> 6;
        return ( a * 67108864.0 + b ) * (1.0/9007199254740992.0);  // by Isaku Wada
    }

    inline double DiRand::RandNorm( const double mean, const double stddev )
    {
        // Return a real number from a normal (Gaussian) distribution with given
        // mean and standard deviation by polar form of Box-Muller transformation
        double x, y, r;
        do
        {
            x = 2.0 * Rand() - 1.0;
            y = 2.0 * Rand() - 1.0;
            r = x * x + y * y;
        }
        while ( r >= 1.0 || r == 0.0 );
        double s = sqrt( -2.0 * log(r) / r );
        return mean + x * s * stddev;
    }

    inline double DiRand::operator()()
    {
        return Rand();
    }

    inline void DiRand::Save( uint32* saveArray ) const
    {
        register const uint32 *s = state;
        register uint32 *sa = saveArray;
        register int i = N;
        for( ; i--; *sa++ = *s++ ) {}
        *sa = left;
    }

    inline void DiRand::Load( uint32 *const loadArray )
    {
        register uint32 *s = state;
        register uint32 *la = loadArray;
        register int i = N;
        for( ; i--; *s++ = *la++ ) {}
        left = *la;
        pNext = &state[N-left];
    }

    inline std::ostream& operator<<( std::ostream& os, const DiRand& mtrand )
    {
        register const DiRand::uint32 *s = mtrand.state;
        register int i = mtrand.N;
        for( ; i--; os << *s++ << "\t" ) {}
        return os << mtrand.left;
    }

    inline std::istream& operator>>( std::istream& is, DiRand& mtrand )
    {
        register DiRand::uint32 *s = mtrand.state;
        register int i = mtrand.N;
        for( ; i--; is >> *s++ ) {}
        is >> mtrand.left;
        mtrand.pNext = &mtrand.state[mtrand.N-mtrand.left];
        return is;
    }

    inline DiRand& DiRand::operator=( const DiRand& o )
    {
        if( this == &o ) return (*this);
        register const uint32 *t = o.state;
        register uint32 *s = state;
        register int i = N;
        for( ; i--; *s++ = *t++ ) {}
        left = o.left;
        pNext = &state[N-left];
        return (*this);
    }
}



#endif
