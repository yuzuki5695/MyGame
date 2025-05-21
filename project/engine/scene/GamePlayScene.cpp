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
    //object3d = Object3d::Create("monsterBallUV.obj", Transform({{1.0f, 1.0f, 1.0f}, {0.0f, -1.6f, 0.0f}, {0.0f, 0.0f, 0.0f}}));
    grass = Object3d::Create("terrain.obj", Transform({ {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 30.0f} }));


	// プレイヤーの初期化
	player_ = std::make_unique<Player>();
    player_->Initialize();
    // プレイヤー初期化後にターゲット設定
    CameraManager::GetInstance()->SetTarget(player_->GetObject3d());
    CameraManager::GetInstance()->ToggleCameraMode(true);  // 追従カメラを有効にする


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

    // 更新処理
    player_->Update();


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


    player_->Draw();


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