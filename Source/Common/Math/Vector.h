#pragma once

#include "Engine/Core/DebugLog.h"
#include "Common/Types/Common.h"
#include "Common/Platform/Platform.h"
#include "Common/Math/Math.h"

namespace Volition
{

template<typename T>
class TVector2
{
public:
    union
    {
        T C[2];
        struct
        {
            T X, Y;
        };
    };

public:
    VLN_FINLINE TVector2<T> operator-(const TVector2<T>& Point)
    {
        return {
            X - Point.X,
            Y - Point.Y
        };
    }

    VLN_FINLINE const TVector2<T>& operator+=(const TVector2<T>& Point)
    {
        X += Point.X;
        Y += Point.Y;
        return *this;
    }

    VLN_FINLINE const TVector2<T>& operator-=(const TVector2<T>& Point)
    {
        X -= Point.X;
        Y -= Point.Y;
        return *this;
    }

    VLN_FINLINE void Zero()
    {
        X = Y = 0;
    }

    VLN_FINLINE void Print()
    {
        VLN_LOG_VERBOSE("<Unknown type>");
    }
};

template<>
VLN_FINLINE void TVector2<f32>::Print()
{
    VLN_LOG_VERBOSE("<%f, %f>", X, Y);
}

template<>
VLN_FINLINE void TVector2<i32>::Print()
{
    VLN_LOG_VERBOSE("<%d, %d>", X, Y);
}

using VVector2 = TVector2<f32>;
using VPoint2 = VVector2;

using VVector2i = TVector2<i32>;
using VPoint2i = VVector2i;

class VVector3
{
public:
    union
    {
        f32 C[3];
        struct
        {
            f32 X, Y, Z;
        };
    };

public:
    VLN_FINLINE b32 operator!=(const VVector3& Other)
    {
        return X != Other.X || Y != Other.Y || Z != Other.Z;
    }

    VLN_FINLINE b32 operator==(const VVector3& Other)
    {
        return X == Other.X && Y == Other.Y && Z == Other.Z;
    }

    VLN_FINLINE void Zero()
    {
        X = Y = Z = 0;
    }

    VLN_FINLINE f32 GetLength()
    {
        return Math.Sqrt(X*X + Y*Y + Z*Z);
    }

    VLN_FINLINE f32 GetLengthFast()
    {
        return Math.FastDist3D(X, Y, Z);
    }

    void Normalize()
    {
        const f32 Len = GetLength();

        // Don't do anything on zero vector
        if (Len < Math.Epsilon5)
        {
            return;
        }

        const f32 Inv = 1.0f / Len;
        X *= Inv;
        Y *= Inv;
        Z *= Inv;
    }

    VVector3 GetNormalized()
    {
        const f32 Len = GetLength();

        if (Len < Math.Epsilon5)
        {
            return { 0.0f, 0.0f, 0.0f };
        }

        const f32 Inv = 1.0f / Len;
        return { X * Inv, Y * Inv, Z * Inv };
    }

    VLN_FINLINE static VVector3 GetCross(const VVector3& A, const VVector3& B)
    {
        return {
            (A.Y * B.Z)   - (A.Z * B.Y),
            -((A.X * B.Z) - (A.Z * B.X)),
            (A.X * B.Y)   - (A.Y * B.X)
        };
    }
    VLN_FINLINE static void Cross(const VVector3& A, const VVector3& B, VVector3& R)
    {
        R = {
            (A.Y * B.Z)   - (A.Z * B.Y),
            -((A.X * B.Z) - (A.Z * B.X)),
            (A.X * B.Y)   - (A.Y * B.X)
        };
    }

    VLN_FINLINE void Print()
    {
        VLN_LOG_VERBOSE("<%f, %f, %f>", X, Y, Z);
    }
};

using VPoint3 = VVector3;

class VVector4
{
public:
    union
    {
        f32 C[4];

        struct
        {
            f32 X, Y, Z, W;
        };

        __m128 MC;
    };

public:
    VVector4() = default; 
    constexpr VVector4(f32 InX, f32 InY, f32 InZ, f32 InW = 1.0f) noexcept
        : X(InX), Y(InY), Z(InZ), W(InW)
    {}

    VLN_FINLINE VVector4 operator+(const VVector4& V) const
    {
        return {
            X + V.X, Y + V.Y, Z + V.Z, 1.0f
        };
    }

    VLN_FINLINE VVector4& operator+=(const VVector4& V)
    {
        X += V.X;
        Y += V.Y;
        Z += V.Z;
        W = 1.0f;
        return *this;
    }

    VLN_FINLINE VVector4 operator-(const VVector4& V) const
    {
        return {
            X - V.X, Y - V.Y, Z - V.Z, 1.0f
        };
    }

    VLN_FINLINE VVector4& operator-=(const VVector4& V)
    {
        X -= V.X;
        Y -= V.Y;
        Z -= V.Z;
        W = 1.0f;
        return *this;
    }

    friend VLN_FINLINE static VVector4 operator*(const VVector4& A, f32 S)
    {
        return {
            A.X * S,
            A.Y * S,
            A.Z * S,
            1.0f
        };
    }

    friend VLN_FINLINE static VVector4 operator*(f32 S, const VVector4& A)
    {
        return A * S;
    }

    VLN_FINLINE VVector4 operator/(f32 Scalar) const
    {
        const f32 InvDiv = 1.0f / Scalar;
        return {
            X * Scalar,
            Y * Scalar,
            Z * Scalar,
            1.0f
        };
    }

    VLN_FINLINE VVector4& operator/=(f32 Scalar)
    {
        const f32 InvDiv = 1.0f / Scalar;
        X *= InvDiv;
        Y *= InvDiv;
        Z *= InvDiv;
        return *this;
    }

    VLN_FINLINE void Zero()
    {
        X = Y = Z = 0.0f;
        W = 1.0f;
    }

    VLN_FINLINE void Print()
    {
        VLN_LOG_VERBOSE("<%+f, %+f, %+f, %+f>", X, Y, Z, W);
    }

    VLN_FINLINE void DivByW()
    {
        X /= W;
        Y /= W;
        Z /= W;
    }

    VLN_FINLINE f32 GetLength() const
    {
        return Math.Sqrt(X*X + Y*Y + Z*Z);
    }

    VLN_FINLINE f32 GetLengthFast() const
    {
        return Math.FastDist3D(X, Y, Z);
    }

    VLN_FINLINE void Normalize()
    {
        const f32 Len = GetLength();

        // Don't do anything on zero vector
        if (Len < Math.Epsilon5)
        {
            return;
        }

        const f32 Inv = 1.0f / Len;
        X *= Inv;
        Y *= Inv;
        Z *= Inv;
        W = 1.0f;
    }

    VLN_FINLINE void NormalizeFast()
    {
        const f32 Len = GetLengthFast();

        // Don't do anything on zero vector
        if (Len < Math.Epsilon5)
        {
            return;
        }

        const f32 Inv = 1.0f / Len;
        X *= Inv;
        Y *= Inv;
        Z *= Inv;
        W = 1.0f;
    }

    VLN_FINLINE VVector4 GetNormalized() const
    {
        const f32 Len = GetLength();

        if (Len < Math.Epsilon5)
        {
            return { 0.0f, 0.0f, 0.0f, 1.0f };
        }

        const f32 Inv = 1.0f / Len;
        return { X * Inv, Y * Inv, Z * Inv, 1.0f };
    }

    VLN_FINLINE static f32 Dot(const VVector4& A, const VVector4& B)
    {
        return A.X*B.X + A.Y*B.Y + A.Z*B.Z;
    }

    VLN_FINLINE static VVector4 GetCross(const VVector4& A, const VVector4& B)
    {
        return {
            A.Y * B.Z - A.Z * B.Y,
            -(A.X * B.Z - A.Z * B.X),
            A.X * B.Y - A.Y * B.X,
            1.0f
        };
    }

    VLN_FINLINE static void Cross(const VVector4& A, const VVector4& B, VVector4& R)
    {
        R = {
            A.Y * B.Z - A.Z * B.Y,
            -(A.X * B.Z - A.Z * B.X),
            A.X * B.Y - A.Y * B.X,
            1.0f
        };
    }
};

using VPoint4 = VVector4;

}
