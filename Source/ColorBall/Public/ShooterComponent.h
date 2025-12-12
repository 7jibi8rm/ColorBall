// Copyright (c) 2025 - ShootTester Project

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ShooterComponent.generated.h"

class UMuzzleComponent;
class URepeaterComponent;
class USupplierComponent;

/**
 * 武器射撃制御用のコンポーネント。
 * 砲台・火器管制・給弾装置・弾薬の管理と射撃処理を担当します。
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COLORBALL_API UShooterComponent : public USceneComponent
{
	GENERATED_BODY()

public:

	/// <summary>
	/// コンストラクタ。
	/// ShooterComponentの初期化処理を行います。
	/// </summary>
	UShooterComponent();

	/// <summary>
	/// 指定型のコンポーネントを変更します。
	/// 古いコンポーネントを削除し、新しいコンポーネントを追加します。
	/// </summary>
	/// <typeparam name="T">変更するコンポーネント型</typeparam>
	/// <param name="InAssetPath">新しいコンポーネントのアセットパス</param>
	template<typename T>
	void ChangeComponent(const FString& InAssetPath) {
		// 既存の指定型コンポーネントを全て破棄
		TArray<USceneComponent*> ShooterChildren;
		GetChildrenComponents(true, ShooterChildren);
		for (USceneComponent* Child : ShooterChildren) {
			if (Child && Child->IsA<T>()) {
				Child->DestroyComponent();
			}
		}
		// 新しいコンポーネントを生成・登録・アタッチ
		if (T* NewComponent = NewObject<T>(this, LoadObject<UClass>(nullptr, *InAssetPath))) {
			NewComponent->RegisterComponent();
			NewComponent->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
		}
	}

	/// <summary>
	/// 弾薬クラスを設定します。
	/// 武器から発射される弾薬の種類を指定します。
	/// </summary>
	/// <param name="InAssetPath">設定する弾薬クラスのアセットパス</param>
	void SetBulletActor(const FString& InAssetPath);

	/// <summary>
	/// 弾薬のマテリアルを設定します。
	/// 発射される弾薬の見た目を変更します。
	/// </summary>
	/// <param name="InAssetPath">設定するマテリアルのアセットパス</param>
	void SetBulletMaterial(const FString& InAssetPath);

	/// <summary>
	/// 射撃開始処理。
	/// 連射やバースト射撃の開始トリガーとして呼び出します。
	/// </summary>
	void StartFire();

	/// <summary>
	/// 射撃停止処理。
	/// 連射やバースト射撃の停止トリガーとして呼び出します。
	/// </summary>
	void StopFire();

	/// <summary>
	/// 射撃実行処理。
	/// 実際に弾薬を発射します。
	/// </summary>
	void ExecuteFire();

	/// <summary>
	/// 毎フレーム呼び出されるTick処理。
	/// 射撃タイミングや状態管理を行います。
	/// </summary>
	/// <param name="DeltaTime">前フレームからの経過時間</param>
	/// <param name="TickType">Tickの種類</param>
	/// <param name="ThisTickFunction">Tick関数情報</param>
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	/// <summary>
	/// 指定型のコンポーネントを検索します。
	/// 所有アクターから該当するコンポーネントを取得します。
	/// </summary>
	/// <typeparam name="T">検索するコンポーネント型</typeparam>
	/// <returns>見つかったコンポーネント。なければnullptr</returns>
	template<typename T>
	T* FindComponent() const {
		// アタッチされている子コンポーネントから型一致するものを検索
		for (auto& Child : GetAttachChildren()) {
			if (Child && Child->IsA<T>()) {
				return Cast<T>(Child);
			}
		}
		return nullptr;
	}

	// 経過時間カウンター（射撃間隔管理用）
	float ElapsedTimeCounter{ 0 };

	// 弾薬クラス（発射する弾の種類）
	TSubclassOf<AActor> BulletClass{};

	// 選択中の弾薬マテリアル
	UMaterialInterface* SelectedMaterial{};
};