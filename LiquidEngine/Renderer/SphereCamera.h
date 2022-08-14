#pragma once
#include <DirectXColors.h>
using namespace DirectX;

class SphereCamera
{
public:
	XMVECTOR pos;
	XMVECTOR target;
	XMVECTOR up;

	float mTheta = 0.0f * XM_PI;
	float mPhi = 1.5 * XM_PIDIV4;
	float mRadius = 5.0f;

	XMFLOAT4X4 getViewMatrix() {
		// Convert Spherical to Cartesian coordinates.
		float x = mRadius * sinf(mPhi) * cosf(mTheta);
		float z = mRadius * sinf(mPhi) * sinf(mTheta);
		float y = mRadius * cosf(mPhi);

		// Build the view matrix.
		pos = XMVectorSet(x, y, z, 1.0f);
		target = XMVectorZero();
		up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMFLOAT4X4 result;
		XMStoreFloat4x4(&result, XMMatrixLookAtLH(pos, target, up));
		
		return result;

	}

	
};

