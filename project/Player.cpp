#include "Player.h"
#include<ModelManager.h>

using json = nlohmann::json;

void Player::Initialize() {

    ModelManager::GetInstance()->LoadModel("monsterBallUV.obj");
    
	// jsonファイルからベジェ曲線の制御点を読み込む
	bezierPoints = LoadBezierFromJSON("Resources/bezier_export.json");

    // プレイヤー生成
    object = Object3d::Create("monsterBallUV.obj", Transform({ {1.0f, 1.0f, 1.0f}, {0.0f, -1.6f, 0.0f}, {0.0f, 0.0f, 0.0f} }));

}

void Player::Update() {
    // プレイヤー更新
    object->Update();
}

void Player::Draw() {
    // プレイヤー描画
    object->Draw();
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
void Player::UpdateObjectPosition() {
    int segmentIndex = 0; // 今は1つだけと仮定、複数ならループ管理

    if (segmentIndex + 1 < bezierPoints.size()) {
        const BezierPoint& start = bezierPoints[segmentIndex];
        const BezierPoint& end = bezierPoints[segmentIndex + 1];

        Vector3 pos = BezierInterpolate(
            start.controlPoint,
            start.handleRight,
            end.handleLeft,
            end.controlPoint,
            t
        );

        object->SetTranslate(pos); // 実際のオブジェクトに位置をセット

        t += speed;
        if (t > 1.0f) {
            t = 0.0f;
            segmentIndex++; // 次のセグメントへ
        }
    }
}
