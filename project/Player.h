#pragma once
#include<Object3d.h>
#include "json.hpp"
#include <fstream>
#include <vector>
#include <string>
#include<Transform.h>

class Player
{
public:
	struct BezierPoint {
		Vector3 handleLeft;
		Vector3 controlPoint;
		Vector3 handleRight;
	};

public:
	// 初期化
	void Initialize();
	// 毎フレーム更新
	void Update();
	// 描画
	void Draw();


	std::vector<BezierPoint> LoadBezierFromJSON(const std::string& filePath);
	
	Vector3 BezierInterpolate(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t);

	void UpdateObjectPosition();

	bool Getfige() { return fige; }

private:

	// Object3d
	std::unique_ptr <Object3d> object = nullptr;
	
	std::vector<BezierPoint> bezierPoints;
	float t = 0.0f;
	float speed = 0.01f; // 移動の速さ

	bool fige = false;
};