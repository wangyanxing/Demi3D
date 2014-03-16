/**********************************************************************
This source file is a part of Demi3D
__  ___  __  __  __
|  \|_ |\/||   _)|  \
|__/|__|  ||  __)|__/

Copyright (c) 2013-2014 Demi team
https://github.com/wangyanxing/Demi3D

Released under the MIT License
https://github.com/wangyanxing/Demi3D/blob/master/License.txt

@version 2.3
@author Kojack
@author Transporter
@author Klaim
Extracted From: http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Euler+Angle+Class

***********************************************************************/

#ifndef DiEuler_h__
#define DiEuler_h__

#include "MiscPrerequisites.h"
#include "quaternion.h"
#include "vector3.h"
#include "matrix3.h"

namespace Demi
{
    class DiEuler
    {
    public:
        /// Default constructor.
        DiEuler()
            : mYaw(DiRadian(0.0f)), mPitch(DiRadian(0.0f)), mRoll(DiRadian(0.0f)), mChanged(true)
        {
        }

        /**
        @brief Constructor which takes yaw, pitch and roll values.
        @param y Starting value for yaw
        @param p Starting value for pitch
        @param r Starting value for roll
        */
        DiEuler(const DiRadian &y, const DiRadian &p = DiRadian(0.0f), const DiRadian &r = DiRadian(0.0f))
            : mYaw(y), mPitch(p), mRoll(r), mChanged(true)
        {
        }

        /**
        @brief Constructor which takes yaw, pitch and roll values as floats (DiRadians).
        @param y Starting value for yaw [DiRadian]
        @param p Starting value for pitch [DiRadian]
        @param r Starting value for roll [DiRadian]
        */
        DiEuler(float y, float p = 0.0f, float r = 0.0f)
            : mYaw(DiRadian(y)), mPitch(DiRadian(p)), mRoll(DiRadian(r)), mChanged(true)
        {
        }

        /**
        @brief Default constructor with presets.
        @param DiQuat Calculate starting values from this DiQuat
        */
        explicit DiEuler(const DiQuat &DiQuat)
        {
            fromQuat(DiQuat);
        }

        explicit DiEuler(const DiMat3 &matrix)
        {
            fromMat3(matrix);
        }

        /// Get the Yaw angle.
        inline DiRadian yaw() const { return mYaw; }

        /// Get the Pitch angle.
        inline DiRadian pitch() const { return mPitch; }

        /// Get the Roll angle.
        inline DiRadian roll() const { return mRoll; }

        /**
        @brief Set the yaw.
        @param y New value for yaw
        */
        inline DiEuler &setYaw(DiRadian y)
        {
            mYaw = y;
            mChanged = true;
            return *this;
        }

        /**
        @brief Set the pitch.
        @param p New value for pitch
        */
        inline DiEuler &setPitch(DiRadian p)
        {
            mPitch = p;
            mChanged = true;
            return *this;
        }

        /**
        @brief Set the roll.
        @param r New value for roll
        */
        inline DiEuler &setRoll(DiRadian r)
        {
            mRoll = r;
            mChanged = true;
            return *this;
        }

        /**
        @brief Set all rotations at once.
        @param y New value for yaw
        @param p New value for pitch
        @param r New value for roll
        */
        inline DiEuler &orientation(const DiRadian &y, const DiRadian &p, const DiRadian &r)
        {
            mYaw = y;
            mPitch = p;
            mRoll = r;
            mChanged = true;
            return *this;
        }

        /**
        @brief Apply a relative yaw.
        @param y Angle to add on current yaw
        */
        inline DiEuler &yaw(const DiRadian &y)
        {
            mYaw += y;
            mChanged = true;
            return *this;
        }

        /**
        @brief Apply a relative pitch.
        @param p Angle to add on current pitch
        */
        inline DiEuler &pitch(const DiRadian &p)
        {
            mPitch += p;
            mChanged = true;
            return *this;
        }

        /**
        @brief Apply a relative roll.
        @param r Angle to add on current roll
        */
        inline DiEuler &roll(const DiRadian &r)
        {
            mRoll += r;
            mChanged = true;
            return *this;
        }

        /**
        @brief Apply all relative rotations at once.
        @param y Angle to add on current yaw
        @param p Angle to add on current pitch
        @param r Angle to add on current roll
        */
        inline DiEuler &rotate(const DiRadian &y, const DiRadian &p, const DiRadian &r)
        {
            mYaw += y;
            mPitch += p;
            mRoll += r;
            mChanged = true;
            return *this;
        }

        /// Get a vector pointing forwards.
        inline DiVec3 forward() const { return toQuat() * DiVec3::NEGATIVE_UNIT_Z; }

        /// Get a vector pointing to the right.
        inline DiVec3 right() const { return toQuat() * DiVec3::UNIT_X; }

        /// Get a vector pointing up.
        inline DiVec3 up() const { return toQuat() * DiVec3::UNIT_Y; }

        /**
        @brief Calculate the quaternion of the euler object.
        @details The result is cached, it is only recalculated when the component euler angles are changed.
        */
        inline DiQuat toQuat() const
        {
            if (mChanged)
            {
                mCachedDiQuat = DiQuat(mYaw, DiVec3::UNIT_Y) * DiQuat(mPitch, DiVec3::UNIT_X) * DiQuat(mRoll, DiVec3::UNIT_Z);
                mChanged = false;
            }
            return mCachedDiQuat;
        }

        /// Casting operator. This allows any ogre function that wants a DiQuat to accept a Euler instead.
        inline operator DiQuat() const
        {
            return toQuat();
        }

        /**
        @brief Calculate the current euler angles of a given DiQuat object.
        @param DiQuat DiQuat which is used to calculate current euler angles.
        */
        inline DiEuler &fromQuat(const DiQuat &DiQuat)
        {
            DiMat3 rotmat;
            DiQuat.ToRotationMatrix(rotmat);
            fromMat3(rotmat);
            return *this;
        }

        /**
        @brief Calculate the current euler angles of a given matrix object.
        @param matrix DiMat3 which is used to calculate current euler angles.
        */
        inline DiEuler &fromMat3(const DiMat3 &matrix)
        {
            matrix.ToEulerAnglesYXZ(mYaw, mPitch, mRoll);
            mChanged = true;
            return *this;
        }

        /**
        @brief Set the yaw and pitch to face in the given direction.
        @details The direction doesn't need to be normalised. Roll is always unaffected.
        @param setYaw If false, the yaw isn't changed.
        @param setPitch If false, the pitch isn't changed.
        */
        inline DiEuler &direction(const DiVec3 &v, bool setYaw = true, bool setPitch = true)
        {
            DiVec3 d(v.normalisedCopy());
            if (setPitch)
                mPitch = DiMath::ASin(d.y);
            if (setYaw)
                mYaw = DiMath::ATan2(-d.x, -d.z);
            mChanged = setYaw || setPitch;
            return *this;
        }

        /**
        @brief Normalise the selected rotations to be within the +/-180 degree range.
        @details The normalise uses a wrap around, so for example a yaw of 360 degrees becomes 0 degrees, and -190 degrees becomes 170.
        @param normYaw If false, the yaw isn't normalized.
        @param normPitch If false, the pitch isn't normalized.
        @param normRoll If false, the roll isn't normalized.
        */
        inline DiEuler &normalise(bool normYaw = true, bool normPitch = true, bool normRoll = true)
        {
            if (normYaw)
                wrapAngle(mYaw);

            if (normPitch)
                wrapAngle(mPitch);

            if (normRoll)
                wrapAngle(mRoll);

            return *this;
        }

        /**
        @brief Return the relative euler angles required to rotate from the current forward direction to the specified dir vector.
        @details The result euler can then be added to the current euler to immediately face dir.
        The rotation won't flip upside down then roll instead of a 180 degree yaw.
        @param setYaw If false, the angle is set to 0. If true, the angle is calculated.
        @param setPitch If false, the angle is set to 0. If true, the angle is calculated.
        @param shortest If false, the full value of each angle is used. If true, the angles are normalised and the shortest
        rotation is found to face the correct direction. For example, when false a yaw of 1000 degrees and a dir of
        (0,0,-1) will return a -1000 degree yaw. When true, the same yaw and dir would give 80 degrees (1080 degrees faces
        the same way as (0,0,-1).
        */
        inline DiEuler rotationTo(const DiVec3 &dir, bool setYaw = true, bool setPitch = true, bool shortest = true) const
        {
            DiEuler t1;
            DiEuler t2;
            t1.direction(dir, setYaw, setPitch);
            t2 = t1 - *this;
            if (shortest && setYaw)
            {
                t2.normalise();
            }
            return t2;
        }

        /// Clamp the yaw angle to a range of +/-limit.
        inline DiEuler &limitYaw(const DiRadian &limit)
        {
            limitAngle(mYaw, limit);
            return *this;
        }

        /// Clamp the pitch angle to a range of +/-limit.
        inline DiEuler &limitPitch(const DiRadian &limit)
        {
            limitAngle(mPitch, limit);
            return *this;
        }

        /// Clamp the roll angle to a range of +/-limit.
        inline DiEuler &limitRoll(const DiRadian &limit)
        {
            limitAngle(mRoll, limit);
            return *this;
        }

        /// Add two euler objects.
        inline DiEuler operator+(const DiEuler &rhs) const
        {
            return DiEuler(mYaw + rhs.mYaw, mPitch + rhs.mPitch, mRoll + rhs.mRoll);
        }

        /**
        @brief Subtract two euler objects.
        @details This finds the difference as relative angles.
        */
        inline DiEuler operator-(const DiEuler &rhs) const
        {
            return DiEuler(mYaw - rhs.mYaw, mPitch - rhs.mPitch, mRoll - rhs.mRoll);
        }

        /// Interpolate the euler angles by rhs.
        inline DiEuler operator*(float rhs) const
        {
            return DiEuler(mYaw * rhs, mPitch * rhs, mRoll * rhs);
        }

        /// Interpolate the euler angle by lhs.
        inline friend DiEuler operator*(float lhs, const DiEuler &rhs)
        {
            return DiEuler(lhs * rhs.mYaw, lhs * rhs.mPitch, lhs * rhs.mRoll);
        }

        /**
        @brief Multiply two eulers.
        @details This has the same effect as multiplying DiQuats.
        @returns The result is a DiQuat.
        */
        inline DiQuat operator*(DiEuler rhs) const
        {
            DiEuler e1(*this), e2(rhs);
            return e1.toQuat()*e2.toQuat();
        }


        /// Apply the euler rotation to the vector rhs.
        inline DiVec3 operator*(const DiVec3 &rhs) const
        {
            return toQuat() * rhs;
        }


        /// Copy assignment operator (Euler)
        inline DiEuler& operator=(const DiEuler& src)
        {
            orientation(src.yaw(), src.pitch(), src.roll());
            return *this;
        }

        /// Copy assignment operator (DiQuat)
        inline DiEuler& operator=(const DiQuat &DiQuat)
        {
            fromQuat(DiQuat);
            return *this;
        }

        /// Copy assignment operator (DiMat3)
        inline DiEuler& operator=(const DiMat3 &matrix)
        {
            fromMat3(matrix);
            return *this;
        }

        inline friend bool operator==(const DiEuler& left, const DiEuler& right)
        {
            return left.mYaw == right.mYaw
                && left.mPitch == right.mPitch
                && left.mRoll == right.mRoll
                ;
        }

        inline friend bool operator!=(const DiEuler& left, const DiEuler& right)
        {
            return !(left == right);
        }

        inline friend bool sameOrientation(const DiEuler& left, const DiEuler& right)
        {
            // I'm comparing resulting vectors to avoid having to compare angles that are the same but in different values.
            // Only the resulting oriented vectors floatly have any meaning in the end.
            return left.forward().positionEquals(right.forward())
                && left.up().positionEquals(right.up())
                ;
        }

    protected:
        DiRadian mYaw;                           //!< Rotation around the Y axis.
        DiRadian mPitch;                            //!< Rotation around the X axis.
        DiRadian mRoll;                           //!< Rotation around the Z axis.
        mutable DiQuat mCachedDiQuat;       //!< Cached DiQuat equivalent of this euler object.
        mutable bool mChanged;                  //!< Is the cached DiQuat out of date?

        inline void wrapAngle(DiRadian& angle)
        {
            float rangle = angle.valueRadians();
            if (rangle < -DiMath::PI)
            {
                rangle = fmod(rangle, -DiMath::TWO_PI);
                if (rangle < -DiMath::PI)
                {
                    rangle += DiMath::TWO_PI;
                }
                angle = rangle;
                mChanged = true;
            }
            else if (rangle > DiMath::PI)
            {
                rangle = fmod(rangle, DiMath::TWO_PI);
                if (rangle > DiMath::PI)
                {
                    rangle -= DiMath::TWO_PI;
                }
                angle = rangle;
                mChanged = true;
            }

        }

        inline void limitAngle(DiRadian& angle, const DiRadian& limit)
        {
            if (angle > limit)
            {
                angle = limit;
                mChanged = true;
            }
            else if (angle < -limit)
            {
                angle = -limit;
                mChanged = true;
            }
        }
    };
}

#endif
