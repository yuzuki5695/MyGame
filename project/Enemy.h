#pragma once
#include <Vector3.h>
#include<Object3d.h>
#include<Transform.h>
#include<random>

class Enemy {
public:
	// 初期化
	void Initialize();
	// 毎フレーム更新
	void Update();
	// 描画
	void Draw();

private:
	// Object3d
	std::unique_ptr<Object3d> object;
    
	
	Vector3 position_;
    float radius_ = 1.0f; // 敵の半径
    bool active_ = true;

	// ランダムエンジン
	std::mt19937 randomEngine;
public:

    bool IsActive() const { return active_; }
    Vector3 GetPosition() const { return position_; }
    float GetRadius() const { return radius_; }
	void SetInactive() { active_ = false; }

};
