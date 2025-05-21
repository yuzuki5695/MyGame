#include "Enemy.h"

void Enemy::Initialize() {
    // 乱数エンジンを初期化
    std::random_device rd;// 乱数生成器
    randomEngine = std::mt19937(rd());
    std::uniform_real_distribution<float> distX(-12.0f, 12.0f);
    std::uniform_real_distribution<float> distY(3.0f, 12.0f);
    std::uniform_real_distribution<float> distZ(60.0f, 150.0f);
	position_ = { distX(randomEngine), distY(randomEngine), distZ(randomEngine) };
    object = Object3d::Create("uvChecker.obj", Transform({ {1.0f, 1.0f, 1.0f},     {0.0f, 0.0f, 0.0f}, position_.x,position_.y,position_.z }));
}

void Enemy::Update() {

    // 位置をobjectから取得して同期する
    //position_ = object->GetWorldPosition(); // ← 追加
	object->SetTranslate(position_); // ← 追加
    object->Update();
    
}

void Enemy::Draw() {
   
    object->Draw();
    
}