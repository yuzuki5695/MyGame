#include "CameraManager.h"
#include <Object3dCommon.h>
#include <ParticleCommon.h>
#include <MatrixVector.h>
#ifdef USE_IMGUI
#include <ImGuiManager.h>
#endif // USE_IMGUI

using namespace MatrixVector;

// 静的メンバ変数の定義
std::unique_ptr<CameraManager> CameraManager::instance = nullptr;

// シングルトンインスタンスの取得
CameraManager* CameraManager::GetInstance() {
    if (!instance) {
        instance = std::make_unique<CameraManager>();
    }
    return instance.get();
}

// 終了
void CameraManager::Finalize() {
    instance.reset();  // `delete` 不要
}

void CameraManager::Initialize() {
    // 初期化時
    defaultCamera_ = new Camera(); 
    defaultCamera_->SetTranslate(Vector3(0.0f, 3.0f, -30.0f));
    defaultCamera_->SetRotate(Vector3(0.0f, 0.0f, 0.0f));
    Object3dCommon::GetInstance()->SetDefaultCamera(defaultCamera_);
    ParticleCommon::GetInstance()->SetDefaultCamera(defaultCamera_);

    followCamera_ = new Camera();
}

void CameraManager::Update() {

    // 追従カメラの更新（ターゲットが設定されていて、追従フラグが有効なとき）
    if (useFollowCamera_ && target_) {
        // ターゲットのワールド座標を取得
        const Vector3& targetPos = target_->GetTranslate();

        // カメラ座標
        Vector3 cameraPos = targetPos + offset_;

        followCamera_->SetTranslate(cameraPos);

        followCamera_->Update();
    } else {
        // デフォルトカメラの更新
        defaultCamera_->Update();
    }

    // 共通で使うカメラを更新（描画に使うカメラを切り替え）
    if (useFollowCamera_) {
        Object3dCommon::GetInstance()->SetDefaultCamera(followCamera_);
        ParticleCommon::GetInstance()->SetDefaultCamera(followCamera_);
    } else {
        Object3dCommon::GetInstance()->SetDefaultCamera(defaultCamera_);
        ParticleCommon::GetInstance()->SetDefaultCamera(defaultCamera_);
    }
}

void CameraManager::SetTarget(Object3d* target, Vector3 offset) {  
    if (target) {  
       target_ = target;
       offset_ = offset;
       // プレイヤーのワールド回転を考慮してオフセットを変換
       Matrix4x4 rotationMatrix = MakeRotateMatrix(target_->GetRotate());
       Vector3 worldOffset = TransformNormal(offset_, rotationMatrix);

       // プレイヤー位置 + 回転後オフセットをカメラ位置に設定
       followCamera_->SetTranslate(target_->GetTranslate() + worldOffset);
   } else {  
       target_ = nullptr;  
   }  
}

void CameraManager::ToggleCameraMode(bool followMode) {
    useFollowCamera_ = followMode;
}

void CameraManager::DrawImGui() {
#ifdef USE_IMGUI
    bool currentMode = useFollowCamera_;
    ImGui::Begin("CameraManager");
    if (ImGui::Checkbox("Follow Camera Mode", &currentMode)) {
        ToggleCameraMode(currentMode);
    };
    

    // ---デフォルトカメラ ---
    Vector3 defaultCameraRot = defaultCamera_->GetRotate();
    Vector3 defaultCameraPos = defaultCamera_->GetTranslate();
    ImGui::DragFloat3("Default Rotation", &defaultCameraRot.x, 0.01f);
    ImGui::DragFloat3("Default Position", &defaultCameraPos.x, 0.01f);
    defaultCamera_->SetTranslate(defaultCameraPos);
    defaultCamera_->SetRotate(defaultCameraRot);

    // --- ターゲットカメラ ---
    Vector3 followCameraPos = followCamera_->GetTranslate();
    Vector3 followCameraRot = followCamera_->GetRotate();
    ImGui::DragFloat3("Follow Position", &followCameraPos.x, 0.01f);
    ImGui::DragFloat3("Follow Rotation", &followCameraRot.x, 0.01f);
    followCamera_->SetTranslate(followCameraPos);
    followCamera_->SetRotate(followCameraRot);

	ImGui::End();
#endif // USE_IMGUI
}