// Fill out your copyright notice in the Description page of Project Settings.

#include "EquipActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

/// <summary>
/// コンストラクタ。
/// EquipActorの初期化処理を行います。
/// </summary>
AEquipActor::AEquipActor()
{
	// 毎フレームTickを有効化
	PrimaryActorTick.bCanEverTick = true;
	{
		auto& EquipItems = SelectableEquips[static_cast<int32>(EEquipIndex::Muzzle)];
		EquipItems.Items.Add(FEquipItem(TEXT("前方砲台"), TEXT("/Script/ColorBall.SimpleMuzzleComponent")));
		EquipItems.Items.Add(FEquipItem(TEXT("散弾砲台"), TEXT("/Script/ColorBall.SpreaderMuzzleComponent")));
		EquipItems.Items.Add(FEquipItem(TEXT("ツイン砲台"), TEXT("/Script/ColorBall.TwinMuzzleComponent")));
		EquipItems.Items.Add(FEquipItem(TEXT("マルチウェイ砲台"), TEXT("/Script/ColorBall.WayMuzzleComponent")));
		EquipItems.Items.Add(FEquipItem(TEXT("円周囲砲台"), TEXT("/Script/ColorBall.AroundMuzzleComponent")));
	}

	{
		auto& EquipItems = SelectableEquips[static_cast<int32>(EEquipIndex::Repeater)];
		EquipItems.Items.Add(FEquipItem(TEXT("単発発射"), TEXT("/Script/ColorBall.SingleRepeaterComponent")));
		EquipItems.Items.Add(FEquipItem(TEXT("連射"), TEXT("/Script/ColorBall.AutoRepeaterComponent")));
		EquipItems.Items.Add(FEquipItem(TEXT("高速連射"), TEXT("/Script/ColorBall.RapidRepeaterComponent")));
		EquipItems.Items.Add(FEquipItem(TEXT("高速連射"), TEXT("/Script/ColorBall.RapidRepeaterComponent")));
		EquipItems.Items.Add(FEquipItem(TEXT("３点バースト"), TEXT("/Script/ColorBall.BurstRepeaterComponent")));
		EquipItems.Items.Add(FEquipItem(TEXT("バースト連射"), TEXT("/Script/ColorBall.RapidBurstRepeaterComponent")));
		EquipItems.Items.Add(FEquipItem(TEXT("砲台数バースト連射"), TEXT("/Script/ColorBall.MuzzleBurstRepeaterComponent")));
	}

	{
		auto& EquipItems = SelectableEquips[static_cast<int32>(EEquipIndex::Supplier)];
		EquipItems.Items.Add(FEquipItem(TEXT("全砲台給弾"), TEXT("/Script/ColorBall.FullSupplierComponent")));
		EquipItems.Items.Add(FEquipItem(TEXT("順次給弾（降順）"), TEXT("/Script/ColorBall.OrderSupplierComponent")));
		EquipItems.Items.Add(FEquipItem(TEXT("順次給弾（昇順）"), TEXT("/Script/ColorBall.ReverseSupplierComponent")));
		EquipItems.Items.Add(FEquipItem(TEXT("ランダム給弾"), TEXT("/Script/ColorBall.RandomSupplierComponent")));
		EquipItems.Items.Add(FEquipItem(TEXT("全ランダム給弾"), TEXT("/Script/ColorBall.FullRandomSupplierComponent")));
	}

	{
		auto& BulletEquip = SelectableEquips[static_cast<int32>(EEquipIndex::Bullet)];
		BulletEquip.Items.Add(FEquipItem(TEXT("球体プリミティブ"), TEXT("/Script/Engine.BlueprintGeneratedClass'/Game/BP_Ball.BP_Ball_C'")));
	}

	{
		auto& MaterialEquip = SelectableEquips[static_cast<int32>(EEquipIndex::Material)];
		MaterialEquip.Items.Add(FEquipItem(TEXT("レッド"), TEXT("/Script/Engine.Material'/Game/Materials/M_Red.M_Red'")));
		MaterialEquip.Items.Add(FEquipItem(TEXT("グリーン"), TEXT("/Script/Engine.Material'/Game/Materials/M_Green.M_Green'")));
		MaterialEquip.Items.Add(FEquipItem(TEXT("ブルー"), TEXT("/Script/Engine.Material'/Game/Materials/M_Blue.M_Blue'")));
		MaterialEquip.Items.Add(FEquipItem(TEXT("イエロー"), TEXT("/Script/Engine.Material'/Game/Materials/M_Yellow.M_Yellow'")));
	}
}

/// <summary>
/// デストラクタ。
/// EquipActorの終了処理を行います。
/// </summary>
AEquipActor::~AEquipActor()
{
}

/// <summary>
/// 指定したメニュータブに対する選択時コールバック関数を設定します。
/// </summary>
/// <param name="InEquipIndex">メニュータブのインデックス</param>
/// <param name="InCallback">選択時コールバック関数</param> 
void AEquipActor::SetSelectCallback(const AEquipActor::EEquipIndex InEquipIndex, TFunction<void(const FString&)> InCallback)
{
	SelectableEquips[static_cast<int32>(InEquipIndex)].SelectCallback = InCallback;
}

/// <summary>
/// 現在選択中の各インデックスをリセットします。
/// メニューの選択状態を初期化します。
/// 各種コールバックも呼び出します。
/// </summary>
void AEquipActor::ResetCurrentIndices()
{
	// 各メニューのインデックスを0にリセットし、コールバックを呼び出す
	for (int32 i = 0; i < static_cast<int32>(EEquipIndex::Num); ++i) {
		SelectableEquips[i].CurrentIndex = 0;
		SelectableEquips[i].ExecuteCallback();
	}
}

/// <summary>
/// アクター生成・開始時に呼び出されます。
/// ImGuiのTickデリゲートを登録し、UI描画の初期化を行います。
/// </summary>
void AEquipActor::BeginPlay()
{
	Super::BeginPlay();

	// ImGuiTick関数をOnWorldDebugデリゲートにバインド
	FImGuiDelegates::OnWorldDebug().AddUObject(this, &AEquipActor::ImGuiTick);
}

/// <summary>
/// アクター終了時またはレベル遷移時に呼び出されます。
/// ImGuiのTickデリゲートを解除し、UI描画の後処理を行います。
/// </summary>
/// <param name="EndPlayReason">終了理由</param>
void AEquipActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// ImGuiTick関数をOnWorldDebugデリゲートから解除
	FImGuiDelegates::OnWorldDebug().RemoveAll(this);
}

/// <summary>
/// アクター毎フレーム更新処理。
/// 毎フレームごとにUIや選択状態の更新を行います。
/// </summary>
/// <param name="DeltaTime">経過時間</param>
void AEquipActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// プレイヤーコントローラー取得
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		// 各キー入力でインデックスを変更し、コールバックを呼び出す
		if (PC->WasInputKeyJustPressed(EKeys::One))
		{
			auto& EquipItems = SelectableEquips[static_cast<int32>(EEquipIndex::Muzzle)];
			EquipItems.CurrentIndex++;
			EquipItems.CurrentIndex %= EquipItems.Items.Num();
			EquipItems.ExecuteCallback();
		}
		if (PC->WasInputKeyJustPressed(EKeys::Two))
		{
			auto& EquipItems = SelectableEquips[static_cast<int32>(EEquipIndex::Repeater)];
			EquipItems.CurrentIndex++;
			EquipItems.CurrentIndex %= EquipItems.Items.Num();
			EquipItems.ExecuteCallback();
		}
		if (PC->WasInputKeyJustPressed(EKeys::Three))
		{
			auto& EquipItems = SelectableEquips[static_cast<int32>(EEquipIndex::Supplier)];
			EquipItems.CurrentIndex++;
			EquipItems.CurrentIndex %= EquipItems.Items.Num();
			EquipItems.ExecuteCallback();
		}

		if (PC->WasInputKeyJustPressed(EKeys::Four))
		{
			auto& EquipItems = SelectableEquips[static_cast<int32>(EEquipIndex::Material)];
			EquipItems.CurrentIndex++;
			EquipItems.CurrentIndex %= EquipItems.Items.Num();
			EquipItems.ExecuteCallback();
		}

		// 各メニューのインデックスをランダムに設定し、コールバックを呼び出す
		if (PC->WasInputKeyJustPressed(EKeys::Five))
		{
			for (int32 i = 0; i < static_cast<int32>(EEquipIndex::Num); ++i)
			{
				SelectableEquips[i].CurrentIndex = FMath::RandRange(0, SelectableEquips[i].Items.Num() - 1);
				SelectableEquips[i].ExecuteCallback();
			}
		}
	}
}

/// <summary>
/// ImGui描画用Tick関数。
/// ImGuiによるUI表示やテスト制御を行います。
/// </summary>
void AEquipActor::ImGuiTick()
{
	// ImGuiウィンドウサイズを一度だけ設定
	ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);

	// ImGuiウィンドウ開始
	ImGui::Begin("装備メニュー");

	{
		const FString Title = TEXT("砲台レイアウト");
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), TCHAR_TO_UTF8(*Title));
		ImGui::BeginChild(TCHAR_TO_UTF8(*Title), ImVec2(280, 60), true);
		const auto& EquipItems = SelectableEquips[static_cast<int32>(EEquipIndex::Muzzle)];
		ImGui::Text("名前: %s", TCHAR_TO_UTF8(*EquipItems.Items[EquipItems.CurrentIndex].Name));
		ImGui::Text("解説: %s", TCHAR_TO_UTF8(*EquipItems.Items[EquipItems.CurrentIndex].Explain));
		ImGui::EndChild();
	}

	{
		const FString Title = TEXT("発射システム");
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), TCHAR_TO_UTF8(*Title));
		ImGui::BeginChild(TCHAR_TO_UTF8(*Title), ImVec2(280, 60), true);
		const auto& EquipItems = SelectableEquips[static_cast<int32>(EEquipIndex::Repeater)];
		ImGui::Text("名前: %s", TCHAR_TO_UTF8(*EquipItems.Items[EquipItems.CurrentIndex].Name));
		ImGui::Text("解説: %s", TCHAR_TO_UTF8(*EquipItems.Items[EquipItems.CurrentIndex].Explain));
		ImGui::EndChild();
	}

	{
		const FString Title = TEXT("給弾システム");
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), TCHAR_TO_UTF8(*Title));
		ImGui::BeginChild(TCHAR_TO_UTF8(*Title), ImVec2(280, 60), true);
		const auto& EquipItems = SelectableEquips[static_cast<int32>(EEquipIndex::Supplier)];
		ImGui::Text("名前: %s", TCHAR_TO_UTF8(*EquipItems.Items[EquipItems.CurrentIndex].Name));
		ImGui::Text("解説: %s", TCHAR_TO_UTF8(*EquipItems.Items[EquipItems.CurrentIndex].Explain));
		ImGui::EndChild();
	}

	{
		const FString Title = TEXT("ボールタイプ");
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), TCHAR_TO_UTF8(*Title));
		ImGui::BeginChild(TCHAR_TO_UTF8(*Title), ImVec2(280, 60), true);
		const auto& EquipItems = SelectableEquips[static_cast<int32>(EEquipIndex::Material)];
		ImGui::Text("名前: %s", TCHAR_TO_UTF8(*EquipItems.Items[EquipItems.CurrentIndex].Name));
		ImGui::Text("解説: %s", TCHAR_TO_UTF8(*EquipItems.Items[EquipItems.CurrentIndex].Explain));
		ImGui::EndChild();
	}
	ImGui::End();
}