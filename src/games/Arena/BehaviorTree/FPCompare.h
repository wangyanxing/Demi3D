/************************************************************************/
/*                                                                      */
/* This file was adapted from libbehavior                               */
/* Website: https://code.google.com/p/libbehavior/                      */
/* License: http://opensource.org/licenses/BSD-3-Clause                 */
/*                                                                      */
/* Using by Demi3D                                                      */
/*  __  ___  __  __  __                                                 */
/* |  \|_ |\/||   _)|  \                                                */
/* |__/|__|  ||  __)|__/                                                */
/*                                                                      */
/* https://github.com/wangyanxing/Demi3D                                */
/*                                                                      */
/************************************************************************/

#ifndef FP_COMPARE_H_
#define FP_COMPARE_H_

bool AlmostEqualUlps(float A, float B, int maxUlps);
bool AlmostEqual2sComplement(float A, float B, int maxUlps);
bool AlmostEqualUlpsFinal(float A, float B, int maxUlps);

#endif