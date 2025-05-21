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

void GamePlayScene::Finalize() {
}

void GamePlayScene::Initialize() {

    camera = std::make_unique<Camera>();
    camera->SetRotate(Vector3(0.0f, 0.0f, 0.0f));
    camera->SetTranslate(Vector3(0.0f, 0.0f, -30.0f));
    Object3dCommon::GetInstance()->SetDefaultCamera(camera.get());

	CameraManager::GetInstance()->Initialize();

    // カメラの初期化
	//CameraManager::GetInstance()->Initialize();

    // テクスチャを読み込む
    TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");
    TextureManager::GetInstance()->LoadTexture("Resources/monsterBall.png");
    TextureManager::GetInstance()->LoadTexture("Resources/circle.png");
    TextureManager::GetInstance()->LoadTexture("Resources/grass.png");
    TextureManager::GetInstance()->LoadTexture("Resources/circle2.png");

    // .objファイルからモデルを読み込む
    ModelManager::GetInstance()->LoadModel("plane.obj");
    ModelManager::GetInstance()->LoadModel("axis.obj");
    ModelManager::GetInstance()->LoadModel("monsterBallUV.obj");
    ModelManager::GetInstance()->LoadModel("fence.obj");
    ModelManager::GetInstance()->LoadModel("terrain.obj");

    // オブジェクト作成
    grass = Object3d::Create("terrain.obj", Transform({ {1.0f, 1.0f, 10.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 70.0f} }));


	// プレイヤーの初期化
	player_ = std::make_unique<Player>();
    player_->Initialize();
    // プレイヤー初期化後にターゲット設定
    CameraManager::GetInstance()->SetTarget(player_->GetObject3d());
    CameraManager::GetInstance()->ToggleCameraMode(true);  // 追従カメラを有効にする


	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize();
}

void GamePlayScene::Update() {
#pragma region  ImGuiの更新処理開始    
    // object3d
    grass->DebugUpdata("Grass");

	CameraManager::GetInstance()->DrawImGui();
    //camera->DebugUpdate();

#pragma endregion ImGuiの更新処理終了 
    /*-------------------------------------------*/
    /*--------------Cameraの更新処理---------------*/
    /*------------------------------------------*/

    // 毎フレームの更新
    CameraManager::GetInstance()->Update();

    //camera->Update();


#pragma region 全てのObject3d個々の更新処理
    //for (Bullet* b : bullets_) {
    //    b->Update();
    //}

    //for (Enemy* e : enemys_) {
    //    e->Update();
    //}

    //CheckBulletEnemyCollisions();  // 当たり判定
    //CleanupInactiveObjects();      // 不要なオブジェクト削除

    // 更新処理
    
    // プレイヤー
    player_->Update();

    // 敵
    enemy_->Update();


    grass->Update();
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

    // プレイヤー
    player_->Draw();

    // 敵
    enemy_->Draw();


    grass->Draw();

    // パーティクルの描画準備。パーティクルの描画に共通のグラフィックスコマンドを積む 
    ParticleCommon::GetInstance()->Commondrawing();

    ParticleManager::GetInstance()->Draw();

#pragma endregion 全てのObject3d個々の描画処理

#pragma region 全てのSprite個々の描画処理
    // Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
    SpriteCommon::GetInstance()->Commondrawing();

#pragma endregion 全てのSprite個々の描画処理
}
//
//void GamePlayScene::CheckBulletEnemyCollisions() {
//    for (Bullet* bullet : bullets_) {
//        if (!bullet->IsActive()) continue;
//
//        for (Enemy* enemy : enemys_) {
//            if (!enemy->IsActive()) continue;
//
//            float dist = Length(bullet->GetPosition() - enemy->GetPosition());
//            float collisionDist = bullet->GetRadius() + enemy->GetRadius();
//
//            if (dist <= collisionDist) {
//                bullet->SetInactive(); // 弾を非アクティブに
//                enemy->SetInactive();  // 敵も非アクティブに
//                break; // 一発で1体だけ倒す場合はbreak
//            }
//        }
//    }
//}
//
//void GamePlayScene::CleanupInactiveObjects() {
//    // bullets_ を非アクティブなものだけ削除
//    bullets_.erase(std::remove_if(bullets_.begin(), bullets_.end(),
//        [](Bullet* b) {
//            if (!b->IsActive()) {
//                delete b;
//                return true;
//            }
//            return false;
//        }), bullets_.end());
//
//    // enemys_ も同様に
//    enemys_.erase(std::remove_if(enemys_.begin(), enemys_.end(),
//        [](Enemy* e) {
//            if (!e->IsActive()) {
//                delete e;
//                return true;
//            }
//            return false;
//        }), enemys_.end());
//}
