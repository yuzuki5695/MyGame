#pragma once
#include<Object3d.h>
#include "json.hpp"
#include <fstream>
#include <vector>
#include <string>
#include<Transform.h>
#include "Bullet.h"
#include<map>

class Player
{
public:
	struct BezierPoint {
		Vector3 handleLeft;
		Vector3 controlPoint;
		Vector3 handleRight;
	};

	struct BezierCurve {
		std::string name;
		std::vector<BezierPoint> points;
	};

public:
	~Player();

	// 初期化
	void Initialize();
	// 毎フレーム更新
	void Update();
	// 描画
	void Draw();


	std::vector<BezierCurve> LoadBezierCurvesFromJSON(const std::string& filePath);

	Vector3 BezierInterpolate(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t);

	Vector3 UpdateObjectPosition();

	bool Getfige() { return fige; }

	void attachBullet();


	// 弾リストを取得
	const std::vector<Bullet*>& GetBullets() const { return bullets_; }


	bool CheckAndRotateAtCurve1(float deltaTime);

	float EaseInOutSine(float t) {
		return -(cosf(3.14159265f * t) - 1.0f) / 2.0f;
	}

	float EaseInOutCubic(float t) {
		return t < 0.5f ? 4.0f * t * t * t : 1.0f - pow(-2.0f * t + 2.0f, 3) / 2.0f;
	}


private:

	// Object3d
	std::unique_ptr <Object3d> object = nullptr;

	std::vector<BezierCurve> bezierCurves;
	int currentCurveIndex = 0;
	int currentSegmentIndex = 0;
	float t = 0.0f;
	float speed = 0.001f;
	// 回転フラグ
	bool hasRotatedOnCurve1 = false;


	bool isRotating = false;
	float rotationT = 0.0f;
	float rotationDuration = 3.0f; // 1秒かけて回転（必要に応じて調整）
	Vector3 rotateStart;
	Vector3 rotateEnd;






	bool isActive_ = true; // 初期状態ではアクティブ











	// カーブ開始フラグ
	bool fige = false;

	Transform transform{};
	Vector3 moveOffset;

	Vector3 moveDelta = { 0.0f, 0.0f, 0.0f };


	std::vector<Bullet*> bullets_;
	float bulletTimer_ = 0.0f;                   // 経過時間
	const float bulletInterval_ = 0.5f;         // 30秒ごとに弾を撃てる
	bool canShoot_ = true;                       // 弾を撃てるかどうか
public: // メンバ関数
	Object3d* GetObject3d() { return object.get(); }
	Vector3 GetForwardDirection() const {
		// プレイヤーの正面方向ベクトル（例：Y軸方向に前向きの場合）
		// 必要に応じて回転を考慮したベクトル計算に変えてください
		return Vector3(0.0f, 0.0f, 1.0f);
	}
	std::vector<Bullet*>& GetBullet() { return bullets_; } // 非const版

	bool IsActive() const { return isActive_; }
	void SetInactive() { isActive_ = false; }
};