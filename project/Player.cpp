#include "Player.h"
#include<ModelManager.h>
#ifdef USE_IMGUI
#include<ImGuiManager.h>
#endif // USE_IMGUI
#include<Input.h>

using json = nlohmann::json;

// デストラクタで弾を解放するのも忘れずに
Player::~Player() {
    for (auto b : bullets_) {
        delete b;
    }
    bullets_.clear();
}

void Player::Initialize() {

    ModelManager::GetInstance()->LoadModel("uvChecker.obj");

    transform = { {1.0f, 1.0f, 1.0f}, {0.0f, -1.6f, 0.0f}, {0.0f, 3.0f, 0.0f} };

    // jsonファイルからベジェ曲線の制御点を読み込む
    bezierPoints = LoadBezierFromJSON("Resources/bezier.json");

    // プレイヤー生成
    object = Object3d::Create("uvChecker.obj", transform);

    // 球生成

}

void Player::Update() {
    moveDelta = Vector3(0.0f, 0.0f, 0.0f);
    // キー入力でmoveDeltaに1フレーム分の移動量加算
    if (Input::GetInstance()->Pushkey(DIK_A)) moveDelta.x -= 0.1f;
    if (Input::GetInstance()->Pushkey(DIK_D)) moveDelta.x += 0.1f;
    if (Input::GetInstance()->Pushkey(DIK_W)) moveDelta.y += 0.1f;
    if (Input::GetInstance()->Pushkey(DIK_S)) moveDelta.y -= 0.1f;

    if (fige) {
        // ベジェ曲線の位置を取得
        Vector3 bezierPos = UpdateObjectPosition(); // UpdateObjectPositionを変更してVector3返すようにする
        // キー移動を累積するための変数を用意（例: moveOffset）
        moveOffset = moveOffset + moveDelta;

        // ベジェ曲線位置 + キー操作の移動量 を合成
        transform.translate = bezierPos + moveOffset;
    } else {
        // ベジェ曲線移動OFFのときはキー操作のみ
        transform.translate = transform.translate + moveDelta;
    }

    attachBullet();

    // 弾の更新
    for (auto& b : bullets_) {
        if (b->IsActive()) {
            b->Update();
        }
    }

    // 移動後の位置をObjectに反映
    object->SetTranslate(transform.translate);
    object->SetRotate(transform.rotate);
    object->SetScale(transform.scale);
    // プレイヤー更新
    object->Update();

#ifdef USE_IMGUI
    ImGui::Begin("Player Control");
    ImGui::Checkbox("Follow Bezier", &fige);
    ImGui::SliderFloat("Bezier Speed", &speed, 0.001f, 0.01f, "%.3f");
    ImGui::DragFloat3("Translate", &transform.translate.x, 0.01f);
    ImGui::DragFloat3("Rotate", &transform.rotate.x, 0.01f);
    ImGui::DragFloat3("Scale", &transform.scale.x, 0.01f);
    ImGui::End();
#endif // USE_IMGUI

}

void Player::Draw() {
    // プレイヤー描画
    object->Draw();
    // 弾の描画
    for (auto& b : bullets_) {
        if (b->IsActive()) {
            b->Draw();
        }
    }
}

std::vector<Player::BezierPoint> Player::LoadBezierFromJSON(const std::string& filePath) {
    std::vector<BezierPoint> points;

    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("JSONファイルを開けませんでした");
    }

    json j;
    file >> j;

    for (const auto& pointData : j) {
        BezierPoint pt;
        pt.handleLeft = {
            pointData["handle_left"][0],
            pointData["handle_left"][1],
            pointData["handle_left"][2]
        };
        pt.controlPoint = {
            pointData["control_point"][0],
            pointData["control_point"][1],
            pointData["control_point"][2]
        };
        pt.handleRight = {
            pointData["handle_right"][0],
            pointData["handle_right"][1],
            pointData["handle_right"][2]
        };
        points.push_back(pt);
    }

    return points;
}

Vector3 Player::BezierInterpolate(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t) {
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    Vector3 result;
    result = uuu * p0; // (1 - t)^3 * P0
    result += 3 * uu * t * p1; // 3 * (1 - t)^2 * t * P1
    result += 3 * u * tt * p2; // 3 * (1 - t) * t^2 * P2
    result += ttt * p3; // t^3 * P3

    return result;
}

// 毎フレーム呼ばれる関数内など
Vector3 Player::UpdateObjectPosition() {
    Vector3 Position{};

    int segmentIndex = 0; // 今は1つだけと仮定、複数ならループ管理

    if (segmentIndex + 1 < bezierPoints.size()) {
        const BezierPoint& start = bezierPoints[segmentIndex];
        const BezierPoint& end = bezierPoints[segmentIndex + 1];

        Position = BezierInterpolate(
            start.controlPoint,
            start.handleRight,
            end.handleLeft,
            end.controlPoint,
            t
        );

        t += speed;
        if (t > 1.0f) {
            t = 0.0f;
            segmentIndex++;
        }
    }
    return Position; // ベジェ曲線の終点など適宜返す
}


void Player::attachBullet() {
    bulletTimer_ += 1.0f / 60.0f; // 毎フレーム経過時間を加算（60fps前提）
    // 30秒経過したら発射可能にする
    if (bulletTimer_ >= bulletInterval_) {
        canShoot_ = true;
        bulletTimer_ = 0.0f; // タイマーリセット
    }

    // 弾が撃てるか確認
    if (!canShoot_) return;

    // スペースキーが押されたら弾を撃つ
    if (Input::GetInstance()->Pushkey(DIK_SPACE)) {
        Bullet* newBullet = new Bullet();
        // 発射位置：プレイヤーの現在位置
        Vector3 startPos = transform.translate;
        // 発射方向（プレイヤーの前方）
        Vector3 forward = Vector3(0.0f, 0.0f, 1.0f);
        newBullet->Initialize(startPos, forward, 0.5f);
        bullets_.push_back(newBullet);

        canShoot_ = false; // 発射したら次の発射を禁止
    }
}