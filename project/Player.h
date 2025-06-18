#pragma once
#include<Object3d.h>
#include <fstream>
#include <vector>
#include <string>
#include<Transform.h>

class Player{
public:
	~Player();

	// 初期化
	void Initialize();
	// 毎フレーム更新
	void Update();
	// 描画
	void Draw();


	bool Getfige() { return fige; }

	void attachBullet();


	// 弾リストを取得
	//const std::vector<Bullet*>& GetBullets() const { return bullets_; }

private:
	// Object3d
	std::unique_ptr <Object3d> object = nullptr;
	

	bool fige = false;

	Transform transform{};
	Vector3 moveOffset;

	Vector3 moveDelta = { 0.0f, 0.0f, 0.0f };

public: // メンバ関数
	Object3d* GetObject3d() { return object.get(); }
	Vector3 GetForwardDirection() const {
		// プレイヤーの正面方向ベクトル（例：Y軸方向に前向きの場合）
		// 必要に応じて回転を考慮したベクトル計算に変えてください
		return Vector3(0.0f, 0.0f, 1.0f);
	}
	//std::vector<Bullet*>& GetBullet() { return bullets_; } // 非const版
};