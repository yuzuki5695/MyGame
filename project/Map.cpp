#include "Map.h"
#include<ModelManager.h>
#ifdef USE_IMGUI
#include<ImGuiManager.h>
#endif // USE_IMGUI

void Map::Initialize() {
	ModelManager::GetInstance()->LoadModel("Tube.obj");
	transform = { {1.0f, 1.0f, 1.0f}, {0.0f, -1.6f, 0.0f}, {0.0f, 0.0f, 0.0f} };

	// プレイヤー生成
	map_ = Object3d::Create("Tube.obj", transform);

}

void Map::Update() {

	map_->Update();

}

void Map::Draw() {

	map_->Draw();

}
void Map::DrawImGui() {
#ifdef USE_IMGUI
	ImGui::Begin("MAP Control");
	ImGui::DragFloat3("Translate", &transform.translate.x, 0.01f);
	ImGui::DragFloat3("Rotate", &transform.rotate.x, 0.01f);
	ImGui::DragFloat3("Scale", &transform.scale.x, 0.01f);
	ImGui::End();
#endif // USE_IMGUI
}