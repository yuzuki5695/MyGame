#include "Enemy.h"

void Enemy::Initialize() {

    object.push_back(Object3d::Create(
        "uvChecker.obj",
        Transform({
            {1.0f, 1.0f, 1.0f},              // scale
            {0.0f, 0.0f, 0.0f},              // rotation
            {5.0f, 7.0f, 40.0f}    // position（x方向にずらす）
            })
    ));
}

void Enemy::Update() {
    for (auto& obj : object) {
        obj->Update();
    }
}

void Enemy::Draw() {
    for (auto& obj : object) {
        obj->Draw();
    }
}