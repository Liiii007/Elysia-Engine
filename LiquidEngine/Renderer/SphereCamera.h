#pragma once
#include <DirectXColors.h>
using namespace DirectX;

class SphereCamera
{
public:
	XMFLOAT3 pos;
	XMFLOAT3 target;
	XMFLOAT3 up;

	float mTheta = 0.0f * XM_PI;
	float mPhi = 1.0 * XM_PIDIV4;
	float mRadius = 3.0f;

	XMMATRIX getViewMatrix() {
		// Convert Spherical to Cartesian coordinates.
		float x = mRadius * sinf(mPhi) * cosf(mTheta);
		float z = mRadius * sinf(mPhi) * sinf(mTheta);
		float y = mRadius * cosf(mPhi);

		pos.x = mRadius;
		pos.y = 0;
		pos.z = 0;

		// Build the view matrix.
		XMVECTOR posV = XMVectorSet(x, y, z, 1.0f);
		XMVECTOR targetV = XMVectorZero();
		XMVECTOR upV = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		return XMMatrixLookAtLH(posV, targetV, upV);

	}

	XMFLOAT3 GetPosition() {
		return pos;
	}

	XMFLOAT3 GetDirection() {
		XMFLOAT3 direction;
		direction.x = target.x - pos.x;
		direction.y = target.y - pos.y;
		direction.z = target.z - pos.z;
		return direction;
	}
	
};

