#include "GamePlayScene.h"
#include<TextureManager.h>
#include<ModelManager.h>
#include<SpriteCommon.h>
#include<Object3dCommon.h>
#include<Input.h>
#ifdef USE_IMGUI
#include<ImGuiManager.h>
#endif // USE_IMGUI
#include<SceneManager.h>
#include <ParticleCommon.h>
#include <ParticleManager.h>
#include <numbers>

void GamePlayScene::Finalize() {

}

void GamePlayScene::Initialize() {

    // カメラの初期化
    camera = std::make_unique<Camera>();
    camera->SetTranslate(Vector3(0.0f, 0.0f, -30.0f));
    camera->SetRotate(Vector3(0.0f, 0.0f, 0.0f));
    Object3dCommon::GetInstance()->SetDefaultCamera(camera.get());
    ParticleCommon::GetInstance()->SetDefaultCamera(camera.get());

   CameraManager::GetInstance()->Initialize();

    // テクスチャを読み込む
    TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");
    TextureManager::GetInstance()->LoadTexture("Resources/monsterBall.png");
    TextureManager::GetInstance()->LoadTexture("Resources/circle.png");
    TextureManager::GetInstance()->LoadTexture("Resources/grass.png");
    TextureManager::GetInstance()->LoadTexture("Resources/circle2.png");
    TextureManager::GetInstance()->LoadTexture("Resources/gradationLine.png");

    // .objファイルからモデルを読み込む
    ModelManager::GetInstance()->LoadModel("plane.obj");
    ModelManager::GetInstance()->LoadModel("axis.obj");
    ModelManager::GetInstance()->LoadModel("monsterBallUV.obj");
    ModelManager::GetInstance()->LoadModel("fence.obj");
    ModelManager::GetInstance()->LoadModel("terrain.obj");

    // 音声ファイルを追加
    soundData = SoundLoader::GetInstance()->SoundLoadWave("Resources/Alarm01.wav");

    // 音声プレイフラグ
    soundfige = 0;

    grass = Object3d::Create("terrain.obj", Transform({ {1.0f, 1.0f, 10.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 70.0f} }));

    // パーティクルグループ生成
    ParticleManager::GetInstance()->CreateParticleGroup("Particles", "Resources/uvChecker.png", "plane.obj", VertexType::Model);            // モデルで生成

    // プレイヤーの初期化
    player_ = std::make_unique<Player>();
    player_->Initialize();
    // プレイヤー初期化後にターゲット設定
    CameraManager::GetInstance()->SetTarget(player_->GetObject3d());
    CameraManager::GetInstance()->ToggleCameraMode(true);  // 追従カメラを有効にする

    // 敵を複数体生成
    for (int i = 0; i < 10; ++i) {
        auto enemy = std::make_unique<Enemy>();
        enemy->Initialize();
        enemys_.push_back(std::move(enemy));
    }

    map01_ = std::make_unique<Map>();
    map01_->Initialize();
}

void GamePlayScene::Update() {
#pragma region  ImGuiの更新処理開始
#ifdef USE_IMGUI
    // object3d

    // Camera
    CameraManager::GetInstance()->DrawImGui();


    ImGui::Begin("Enemys");
    int index = 0;
    for (const auto& enemy : enemys_) {
        std::string label = "Enemy " + std::to_string(index) + " Active";
        bool isActive = enemy->IsActive(); // 値を一時変数にコピー
        ImGui::Text("%s: %s", label.c_str(), isActive ? "true" : "false");
        index++;
    }
    ImGui::End();

#endif // USE_IMGUI
#pragma endregion ImGuiの更新処理終了 
    /*-------------------------------------------*/
    /*--------------Cameraの更新処理---------------*/
    /*------------------------------------------*/
    CameraManager::GetInstance()->Update();

#pragma region 全てのObject3d個々の更新処理

    // 更新処理
    //object3d->Update();
    grass->Update();

    // プレイヤー
    player_->Update();


    // 敵
    for (auto& enemy : enemys_) {
        if (enemy->IsActive()) {
            enemy->Update();
        }
    }

    CheckBulletEnemyCollisions();  // 当たり判定
    CleanupInactiveObjects();      // 不要なオブジェクト削除


    map01_->Update();


    // パーティクル
    ParticleManager::GetInstance()->Update();

#pragma endregion 全てのObject3d個々の更新処理

#pragma region 全てのSprite個々の更新処理
    

#pragma endregion 全てのSprite個々の更新処理
#ifdef USE_IMGUI
// ImGuiの描画前準備
    ImGuiManager::GetInstance()->End();
#endif // USE_IMGUI
}

void GamePlayScene::Draw() {

#pragma region 全てのObject3d個々の描画処理
    // 3Dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
    Object3dCommon::GetInstance()->Commondrawing();
    
    map01_->Draw();

    // プレイヤー
    player_->Draw();

    // 敵
    for (auto& enemy : enemys_) {
        if (enemy->IsActive()) {
            enemy->Draw();
        }
    }



    // パーティクルの描画準備。パーティクルの描画に共通のグラフィックスコマンドを積む 
    ParticleCommon::GetInstance()->Commondrawing();
    ParticleManager::GetInstance()->Draw();
#pragma endregion 全てのObject3d個々の描画処理

#pragma region 全てのSprite個々の描画処理
    // Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
    SpriteCommon::GetInstance()->Commondrawing();

#pragma endregion 全てのSprite個々の描画処理
}


void GamePlayScene::CheckBulletEnemyCollisions() {

    // 自弾リストの取得
    const std::vector<Bullet*>& bullets_ = player_->GetBullets();


    for (Bullet* bullet : bullets_) {
        if (!bullet->IsActive()) continue;

        for (const auto& enemy : enemys_) {
            if (!enemy->IsActive()) continue;

            //float dist = Length(bullet->GetPosition() - enemy->GetPosition());
            Vector3 delta = {
                bullet->GetPosition().x - enemy->GetPosition().x
                , bullet->GetPosition().y - enemy->GetPosition().y
                , bullet->GetPosition().z - enemy->GetPosition().z
            };
            float dist = Length(delta);

            float collisionDist = bullet->GetRadius() + enemy->GetRadius();

            if (dist <= collisionDist) {
                bullet->SetInactive(); // 弾を非アクティブに
                enemy->SetInactive();  // 敵も非アクティブに
                break; // 一発で1体だけ倒す場合はbreak
            }
        }
    }
}

void GamePlayScene::CleanupInactiveObjects() {
    // 自弾リストの取得
    std::vector<Bullet*>& bullets_ = player_->GetBullet();

    // bullets_ を非アクティブなものだけ削除
    bullets_.erase(std::remove_if(bullets_.begin(), bullets_.end(),
        [](Bullet* b) {
            if (!b->IsActive()) {
                delete b;
                return true;
            }
            return false;
        }), bullets_.end());

    enemys_.erase(std::remove_if(enemys_.begin(), enemys_.end(),
        [](const std::unique_ptr<Enemy>& e) {
            return !e->IsActive(); // unique_ptr の所有権は move されるので delete は不要
        }), enemys_.end());

}