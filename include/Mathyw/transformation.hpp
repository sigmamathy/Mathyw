#pragma once

#include "./vector.hpp"

namespace Mathyw {

// Translation matrix
Fmat4 Translate(Fvec3 position);

// Rotation matrix (rotate about an axis)
Fmat4 Rotate(float rad, Fvec3 axis = Fvec3(0.0f, 0.0f, 1.0f));

// Scale matrix
Fmat4 Scale(Fvec3 scale);

// 3 dimensional view matrix, look at a certain position
Fmat4 LookAt(Fvec3 position, Fvec3 orientation, Fvec3 up = Fvec3(0.0f, 0.0f, 1.0f));

// Orthogonal projection matrix
Fmat4 OrthogonalProjection(float left, float right, float bottom, float top, float near = -1.0f, float far = 1.0f);

// Orthogonal projection matrix of a screen size
Fmat4 OrthogonalProjection(Fvec2 size);

// Perspective projection matrix
Fmat4 PerspectiveProjection(float fovy, float aspect, float near, float far);

} // !Mathyw