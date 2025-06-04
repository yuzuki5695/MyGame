#pragma once
#include<Object3d.h>


class Map
{
public:
	// 初期化
	void Initialize();
	// 毎フレーム更新
	void Update();
	// 描画
	void Draw();


	void DrawImGui(); // ImGui描画

public:

	std::unique_ptr <Object3d> map_ = nullptr;


	Transform transform{};

public:


};