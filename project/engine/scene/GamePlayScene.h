#pragma once
#include<Sprite.h>
#include<Object3d.h>
#include<Model.h>
#include<SoundPlayer.h>
#include<BaseScene.h>
#include<ParticleEmitter.h>
#include"Player.h"
#include <CameraManager.h>
#include "Enemy.h"
#include"Map.h"

// ゲームプレイシーン
class GamePlayScene : public BaseScene
{
public: // メンバ関数
    // 初期化
    void Initialize() override;
    // 終了
    void Finalize() override;
    // 毎フレーム更新
    void Update() override;
    // 描画
    void Draw() override;


    void CheckBulletEnemyCollisions();
    void CleanupInactiveObjects();

    inline float Length(const Vector3& v) {
        return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    }

private: // メンバ変数
    // オブジェクトデータ
    // camera 
    std::unique_ptr <Camera> camera = nullptr;
    // 地面
    std::unique_ptr <Object3d> grass = nullptr;

    // 音声ファイル
    SoundData soundData;
    // 音声プレイフラグ
    uint32_t soundfige;


    std::unique_ptr <Player> player_;

    std::vector<std::unique_ptr<Enemy>> enemys_;

    std::unique_ptr <Map> map01_;


    // パーティクルエミッタのリスト（死亡エフェクトなどを管理）
    std::vector<std::unique_ptr<ParticleEmitter>> particleEmitters_;
    RandomParameter random_;
};