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

        // 追従カメラの位置をターゲットの後方・上方に設定（例）
        Vector3 offset(0.0f, 3.0f, -30.0f);
        Vector3 cameraPos = targetPos + offset;

        followCamera_->SetTranslate(cameraPos);

        // 方向をターゲットに向ける（ここではY軸固定の簡易的な実装）
        Vector3 toTarget = { targetPos.x - cameraPos.x, targetPos.y - cameraPos.y , targetPos.z - cameraPos.z};
        float angleY = std::atan2(toTarget.x, toTarget.z);
        followCamera_->SetRotate(Vector3(0.0f, angleY, 0.0f));

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

void CameraManager::SetTarget(Object3d* target) {
    target_ = target;
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
    ImGui::DragFloat3("Follow", &followCamera_->GetTranslate().x, 0.01f);
    ImGui::DragFloat3("Default", &defaultCamera_->GetTranslate().x, 0.01f);
	ImGui::End();
#endif // USE_IMGUI
}