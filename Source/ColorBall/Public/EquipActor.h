// Copyright (c) 2025 - ShootTester Project

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColorBall/ImGuiCommon.h"
#include "EquipActor.generated.h"

/**
 * <summary>
 * メニューUIを管理するアクタークラス。
 * ImGuiを用いた各種コンポーネント・弾薬・マテリアルの選択UIを提供し、
 * 選択時のコールバックや状態管理を行います。
 * </summary>
 */
UCLASS()
class COLORBALL_API AEquipActor : public AActor
{
	GENERATED_BODY()

public:

	/// <summary>
	/// メニューのタブを表す列挙型。
	/// 砲台、火器管制機、給弾装置、弾薬、マテリアルの各タブを定義します。
	/// </summary>
	enum class EEquipIndex : int32 {
		Muzzle = 0,		// 砲台
		Repeater,		// 火器管制機
		Supplier,		// 給弾装置
		Bullet,			// 弾薬
		Material,		// マテリアル
		Num				// タブ数
	};

	/// <summary>
	/// コンストラクタ。
	/// EquipActorの初期化処理を行います。
	/// </summary>
	AEquipActor();

	/// <summary>
	/// デストラクタ。
	/// EquipActorの終了処理を行います。
	/// </summary>
	virtual ~AEquipActor();

	/// <summary>
	/// 指定したメニュータブに対する選択時コールバック関数を設定します。
	/// 各タブごとに選択時の処理を外部から登録できます。
	/// </summary>
	/// <param name="InEquipIndex">メニュータブのインデックス</param>
	/// <param name="InCallback">選択時コールバック関数</param>
	void SetSelectCallback(const EEquipIndex InEquipIndex, TFunction<void(const FString&)> InCallback);

	/// <summary>
	/// 現在選択中の各インデックスをリセットします。
	/// 全タブの選択状態を初期化し、最初の項目を選択状態に戻します。
	/// </summary>
	void ResetCurrentIndices();

protected:

	/// <summary>
	/// アクター生成・開始時に呼び出されます。
	/// ImGuiのTickデリゲートを登録し、UI描画の初期化を行います。
	/// </summary>
	virtual void BeginPlay() override;

	/// <summary>
	/// アクター終了時またはレベル遷移時に呼び出されます。
	/// ImGuiのTickデリゲートを解除し、UI描画の後処理を行います。
	/// </summary>
	/// <param name="EndPlayReason">終了理由</param>
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/// <summary>
	/// アクター毎フレーム更新処理。
	/// 毎フレームごとにUIや選択状態の更新を行います。
	/// </summary>
	/// <param name="DeltaTime">経過時間</param>
	virtual void Tick(float DeltaTime) override;

private:

	/// <summary>
	/// ImGui描画用Tick関数。
	/// ImGuiによるUI表示やテスト制御を行います。
	/// メニューの描画・選択・コールバック実行を担当します。
	/// </summary>
	void ImGuiTick();

	/**
	 * <summary>
	 * メニュー項目データ構造体。
	 * 各項目の説明文・アセットパス・表示名を保持します。
	 * </summary>
	 */
	struct FEquipItem {
		FString Explain{};		// 項目の説明文
		FString AssetPath{};	// アセットパス
		FString Name{};			// 表示名
		FEquipItem(FString InExplain, FString InAssetPath)
			: Explain(InExplain), AssetPath(InAssetPath)
		{
			int32 DotIndex = -1;
			if (AssetPath.FindLastChar(TEXT('.'), DotIndex)) {
				Name = AssetPath.Mid(DotIndex + 1);
			}
		}
	};

	/**
	 * <summary>
	 * メニュー項目リストのデータ構造体。
	 * 現在選択中のインデックス、項目リスト、選択時コールバックを保持します。
	 * </summary>
	 */
	struct FEquipItems {
		int32 CurrentIndex{ 0 }; // 現在選択中の項目インデックス
		TArray<FEquipItem> Items{}; // 選択可能な項目のリスト
		TFunction<void(const FString&)> SelectCallback{}; // 項目選択時のコールバック関数

		/// <summary>
		/// 現在の選択項目でコールバックを実行します。
		/// </summary>
		void ExecuteCallback() {
			if (SelectCallback && Items.IsValidIndex(CurrentIndex)) {
				SelectCallback(Items[CurrentIndex].AssetPath);
			}
		}
	};

	// 各メニュータブに対応するメニュー項目のデータ配列
	FEquipItems SelectableEquips[static_cast<int32>(EEquipIndex::Num)]{};
};