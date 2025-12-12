// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ColorBallCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UShooterComponent;
class AEquipActor;

struct FInputActionValue;
struct FShooterParameters;
struct FBulletParameters;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 * <summary>
 * プレイヤーキャラクタークラス。
 * シューティングテスト用キャラクターのカメラ制御、入力処理、射撃機能などを管理します。
 * </summary>
 */
UCLASS(config = Game)
class AColorBallCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	/// <summary>
	/// コンストラクタ。
	/// キャラクターの初期化処理を行います。
	/// </summary>
	AColorBallCharacter();

	/// <summary>
	/// カメラブーム（キャラクターの背後にカメラを配置するためのアーム）を取得します。
	/// </summary>
	/// <returns>カメラブームコンポーネント</returns>
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/// <summary>
	/// フォローカメラ（キャラクターを追従するカメラ）を取得します。
	/// </summary>
	/// <returns>フォローカメラコンポーネント</returns>
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/// <summary>
	/// 砲口コンポーネント選択処理。
	/// メニューから選択された砲口コンポーネントをShooterComponentに反映します。
	/// </summary>
	/// <param name="InAssetPath">選択されたアイテムのアセットパス</param>
	void SelectMuzzleComponent(const FString& InAssetPath);

	/// <summary>
	/// 火器管制機コンポーネント選択処理。
	/// メニューから選択された火器管制機コンポーネントをShooterComponentに反映します。
	/// </summary>
	/// <param name="InAssetPath">選択されたアイテムのアセットパス</param>
	void SelectRepeaterComponent(const FString& InAssetPath);

	/// <summary>
	/// 給弾装置コンポーネント選択処理。
	/// メニューから選択された給弾装置コンポーネントをShooterComponentに反映します。
	/// </summary>
	/// <param name="InAssetPath">選択されたアイテムのアセットパス</param>
	void SelectSupplierComponent(const FString& InAssetPath);

	/// <summary>
	/// 弾丸アクター選択処理。
	/// メニューから選択された弾丸アクターをShooterComponentに反映します。
	/// </summary>
	/// <param name="InAssetPath">選択された弾丸アクターのアセットパス</param>
	void SelectBulletActor(const FString& InAssetPath);

	/// <summary>
	/// 弾丸マテリアル選択処理。
	/// メニューから選択された弾丸マテリアルをShooterComponentに反映します。
	/// </summary>
	/// <param name="InAssetPath">選択されたマテリアルのアセットパス</param>
	void SelectBulletMaterial(const FString& InAssetPath);

protected:
	/// <summary>
	/// アクター生成・開始時に呼び出される初期化処理。
	/// 必要なコンポーネントやメニューの初期化を行います。
	/// </summary>
	virtual void BeginPlay() override;

	/// <summary>
	/// コントローラーが変更されたときに呼び出される処理。
	/// 入力マッピングの再設定などを行います。
	/// </summary>
	virtual void NotifyControllerChanged() override;

	/// <summary>
	/// 入力バインドの設定。
	/// 各種アクション（移動・視点・ジャンプ・射撃など）を入力にバインドします。
	/// </summary>
	/// <param name="PlayerInputComponent">入力コンポーネント</param>
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	/// <summary>
	/// 移動入力処理。
	/// プレイヤーの移動入力を処理します。
	/// </summary>
	/// <param name="Value">入力値</param>
	void Move(const FInputActionValue& Value);

	/// <summary>
	/// 視点入力処理。
	/// プレイヤーの視点（カメラ）操作を処理します。
	/// </summary>
	/// <param name="Value">入力値</param>
	void Look(const FInputActionValue& Value);

	/// <summary>
	/// 射撃開始処理。
	/// プレイヤーが射撃ボタンを押した際の処理を記述します。
	/// </summary>
	/// <param name="Value">入力値</param>
	void StartFire(const FInputActionValue& Value);

	/// <summary>
	/// 射撃停止処理。
	/// プレイヤーが射撃ボタンを離した際の処理を記述します。
	/// </summary>
	/// <param name="Value">入力値</param>
	void StopFire(const FInputActionValue& Value);

	// カメラブーム：キャラクターの背後にカメラを配置するためのアーム。
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	// フォローカメラ：キャラクターを追従するカメラ。
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	// デフォルトの入力マッピングコンテキスト。
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	// ジャンプアクションの入力。
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	// 移動アクションの入力。
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	// 視点アクションの入力。
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	// 射撃アクションの入力。
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	// ShooterComponent：射撃機能を管理するコンポーネント。
	UShooterComponent* ShooterComponent{};

	// EquipActor：メニュー操作用アクター。
	AEquipActor* EquipActor{};
};