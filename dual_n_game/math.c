//TODO: use own sqrt and other
#include <math.h>

#define Minimum(A, B)(A <= B ? A : B)
#define Maximum(A, B)(A >= B ? A : B)

#define Abs(A)(A > 0 ? A : -A)

struct v2
{
  f32 X;
  f32 Y;
};

struct v2
V2Sum(struct v2 First, struct v2 Second)
{
  struct v2 Result = {0};
  Result.X = First.X + Second.X;
  Result.Y = First.Y + Second.Y;

  return Result;
}

struct v2
V2Diff(struct v2 First, struct v2 Second)
{
  struct v2 Result = {0};
  Result.X = First.X - Second.X;
  Result.Y = First.Y - Second.Y;

  return Result;
}

struct v2
V2ScaleBy(struct v2 Vec, f32 Val)
{
  struct v2 Result = {0};
  Result.X = Vec.X * Val;
  Result.Y = Vec.Y * Val;

  return Result;
}

f32
V2Dot(struct v2 First, struct v2 Second)
{
  f32 Result;
  Result = (First.X * Second.X) + (First.Y * Second.Y);

  return Result;
}

f32
V2LengthSquared(struct v2 Vec)
{
  f32 Result;
  Result = (Vec.X * Vec.X) + (Vec.Y * Vec.Y);

  return Result;
}

f32
V2Length(struct v2 Vec)
{
  f32 Result;
  Result = sqrtf((Vec.X * Vec.X) + (Vec.Y * Vec.Y));

  return Result;
}

struct v2
V2Normalize(struct v2 Vec)
{
  struct v2 Result = {0};
  f32 VecLen = V2Length(Vec);
  Result.X = Vec.X / VecLen;
  Result.Y = Vec.Y / VecLen;

  return Result;
}