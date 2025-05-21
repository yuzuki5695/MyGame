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
    defaultCamera_->SetTranslate(Vector3(0.0f, 3.0f, -15.0f));
    defaultCamera_->SetRotate(Vector3(0.1f, 0.0f, 0.0f));
    Object3dCommon::GetInstance()->SetDefaultCamera(defaultCamera_);
    ParticleCommon::GetInstance()->SetDefaultCamera(defaultCamera_);

    followCamera_ = new Camera();
    // 追従カメラの初期位置は例えばターゲットの後ろ少し上に設定
    followCamera_->SetTranslate(Vector3(0, 5, -10));
    followCamera_->SetRotate(Vector3(0, 0, 0));
}

void CameraManager::Update() {
    if (useFollowCamera_ && target_) {
        // 追従カメラの位置をターゲットの位置＋オフセットにする例
        Vector3 targetPos = target_->GetTranslate();
        Vector3 cameraPos = targetPos + Vector3(0, 5, -10); // 高さ5、後方10に追従
        followCamera_->SetTranslate(cameraPos);

        // ターゲットの方向を見るように回転を設定（簡易版）
        Vector3 dir = {
            targetPos.x - cameraPos.x, 
            targetPos.y - cameraPos.y, 
            targetPos.z - cameraPos.z 
        };
        dir = Normalize(dir);
        float yaw = atan2f(dir.x, dir.z);    // Y軸回転（水平回転）
        float pitch = asinf(dir.y);          // X軸回転（上下）
        followCamera_->SetRotate(Vector3(pitch, yaw, 0));

        followCamera_->Update();

    } else {
        // 固定カメラの更新
        defaultCamera_->Update();

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
    }
	ImGui::End();
#endif // USE_IMGUI
}