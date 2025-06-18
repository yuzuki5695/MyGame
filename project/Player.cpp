#include "Player.h"
#include<ModelManager.h>
#ifdef USE_IMGUI
#include<ImGuiManager.h>
#endif // USE_IMGUI
#include<Input.h>
#include <CameraManager.h>


Player::~Player() {
    // 必要ならリソース解放など
}

void Player::Initialize() {

    ModelManager::GetInstance()->LoadModel("uvChecker.obj");
    
	transform = { {1.0f, 1.0f, 1.0f}, {0.0f, -1.6f, 0.0f}, {0.0f, 3.0f, 0.0f} };

    // プレイヤー生成
    object = Object3d::Create("uvChecker.obj", transform);

    moveDelta = Vector3(0.0f, 0.0f, 0.0f);
}

void Player::Update() {
    // キー入力でmoveDeltaに1フレーム分の移動量加算
    if (Input::GetInstance()->Pushkey(DIK_A)) moveDelta.x -= 0.1f;
    if (Input::GetInstance()->Pushkey(DIK_D)) moveDelta.x += 0.1f;
    if (Input::GetInstance()->Pushkey(DIK_W)) moveDelta.y += 0.1f;
    if (Input::GetInstance()->Pushkey(DIK_S)) moveDelta.y -= 0.1f;

        // カメラの現在位置取得
    Camera* camera = CameraManager::GetInstance()->GetActiveCamera();
    if (camera) {
        Vector3 cameraPos = camera->GetTranslate();
        // カメラ相対オフセット位置にプレイヤーを固定
        Vector3 relativeOffset = {0.0f, -6.0f, 30.0f}; // カメラの前に出したいなら +Z方向に
        transform.translate = cameraPos + relativeOffset;
    }

    transform.translate += moveDelta; // キー操作による移動量を加算

    // 移動後の位置をObjectに反映
    object->SetTranslate(transform.translate);
    object->SetRotate(transform.rotate);
    object->SetScale(transform.scale);
    // プレイヤー更新
    object->Update();

#ifdef USE_IMGUI
    ImGui::Begin("Player Control");
    ImGui::DragFloat3("Translate", &transform.translate.x, 0.01f);
    ImGui::DragFloat3("Rotate", &transform.rotate.x, 0.01f);
    ImGui::DragFloat3("Scale", &transform.scale.x, 0.01f);
    ImGui::End();
#endif // USE_IMGUI

}

void Player::Draw() {
    // プレイヤー描画
    object->Draw();
}
