#ifndef PtexUtils_h
#define PtexUtils_h

/* 
   CONFIDENTIAL INFORMATION: This software is the confidential and
   proprietary information of Walt Disney Animation Studios ("Disney").
   This software is owned by Disney and may not be used, disclosed,
   reproduced or distributed for any purpose without prior written
   authorization and license from Disney. Reproduction of any section of
   this software must include this legend and all copyright notices.
   (c) Disney. All rights reserved.
*/

#include "Ptexture.h"

struct PtexUtils : public Ptex {
    static bool isPowerOfTwo(int x)
    {
	return !(x&(x-1));
    }

    static uint32_t ones(uint32_t x)
    {
	// count number of ones
	x = (x & 0x55555555) + ((x >> 1) & 0x55555555); // add pairs of bits
	x = (x & 0x33333333) + ((x >> 2) & 0x33333333); // add bit pairs
	x = (x & 0x0f0f0f0f) + ((x >> 4) & 0x0f0f0f0f); // add nybbles
	x += (x >> 8);		                        // add bytes
	x += (x >> 16);	                                // add words
	return(x & 0xff);
    }

    static uint32_t floor_log2(uint32_t x)
    {
	// floor(log2(n))
	x |= (x >> 1);
	x |= (x >> 2);
	x |= (x >> 4);
	x |= (x >> 8);
	x |= (x >> 16);
	return ones(x>>1);
    }

    static uint32_t ceil_log2(uint32_t x)
    {
	// ceil(log2(n))
	bool isPow2 = isPowerOfTwo(x);
	x |= (x >> 1);
	x |= (x >> 2);
	x |= (x >> 4);
	x |= (x >> 8);
	x |= (x >> 16);
	return ones(x>>1) + !isPow2;
    }

    static double smoothstep(double x, double a, double b)
    {
	if ( x < a ) return 0;
	if ( x >= b ) return 1;
	x = (x - a)/(b - a);
	return x*x * (3 - 2*x);
    }

    static double qsmoothstep(double x, double a, double b)
    {
	// quintic smoothstep (cubic is only C1)

	if ( x < a ) return 0;
	if ( x >= b ) return 1;
	x = (x - a)/(b - a);
	return x*x*x * (10 + x * (-15 + x*6));
    }

    template<typename T>
    static T min(T a, T b) { return a < b ? a : b; }

    template<typename T>
    static T max(T a, T b) { return a > b ? a : b; }

    template<typename T>
    static T clamp(T x, T lo, T hi) { return x < lo ? lo : x > hi ? hi : x; }

    static bool isConstant(const void* data, int stride, int ures, int vres, 
			   int pixelSize);
    static void interleave(const void* src, int sstride, int ures, int vres, 
			   void* dst, int dstride, DataType dt, int nchannels);
    static void deinterleave(const void* src, int sstride, int ures, int vres, 
			     void* dst, int dstride, DataType dt, int nchannels);
    static void encodeDifference(void* data, int size, DataType dt);
    static void decodeDifference(void* data, int size, DataType dt);
    typedef void ReduceFn(const void* src, int sstride, int ures, int vres,
			  void* dst, int dstride, DataType dt, int nchannels);
    static void reduce(const void* src, int sstride, int ures, int vres,
		       void* dst, int dstride, DataType dt, int nchannels);
    static void reduceu(const void* src, int sstride, int ures, int vres,
			void* dst, int dstride, DataType dt, int nchannels);
    static void reducev(const void* src, int sstride, int ures, int vres,
			void* dst, int dstride, DataType dt, int nchannels);
    static void average(const void* src, int sstride, int ures, int vres,
			void* dst, DataType dt, int nchannels);
    static void fill(const void* src, void* dst, int dstride,
		     int ures, int vres, int pixelsize);
    static void copy(const void* src, int sstride, void* dst, int dstride,
		     int nrows, int rowlen);
    static void blend(const void* src, float weight, void* dst, bool flip,
		      int rowlen, DataType dt, int nchannels);
    static void multalpha(void* data, int npixels, DataType dt, int nchannels, int alphachan);
    static void divalpha(void* data, int npixels, DataType dt, int nchannels, int alphachan);

    static void genRfaceids(const FaceInfo* faces, int nfaces, 
			    uint32_t* rfaceids, uint32_t* faceids);
};

#endif
