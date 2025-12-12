// Copyright Epic Games, Inc. All Rights Reserved.

#include "ColorBallCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ShooterComponent.h"
#include "EquipActor.h"
#include "Muzzle/MuzzleComponent.h"
#include "Repeater/RepeaterComponent.h"
#include "Supplier/SupplierComponent.h"
#include "Muzzle/SpreaderMuzzleComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

/// <summary>
/// コンストラクタ：キャラクターの各種初期化処理を行います。
/// コリジョン、カメラ、移動、射撃コンポーネントなどをセットアップします。
/// </summary>
AColorBallCharacter::AColorBallCharacter()
{
	// コリジョンカプセルのサイズ設定
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// コントローラーの回転がキャラクターに影響しないように設定
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// キャラクター移動の設定
	GetCharacterMovement()->bOrientRotationToMovement = true; // 入力方向にキャラクターが向く
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // 回転速度
	GetCharacterMovement()->JumpZVelocity = 700.f; // ジャンプ力
	GetCharacterMovement()->AirControl = 0.35f; // 空中制御
	GetCharacterMovement()->MaxWalkSpeed = 500.f; // 最大歩行速度
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f; // 最小アナログ歩行速度
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f; // 歩行時減速
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f; // 落下時減速

	// カメラブーム（キャラクター背後のカメラアーム）作成
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom")); // カメラのアーム
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // カメラ距離
	CameraBoom->bUsePawnControlRotation = true; // コントローラー回転に追従

	// フォローカメラ作成
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera")); // 追従カメラ
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // カメラ自体は回転しない

	// 射撃コンポーネント作成
	ShooterComponent = CreateDefaultSubobject<UShooterComponent>(TEXT("ShooterComponent")); // 射撃制御コンポーネント
	ShooterComponent->SetupAttachment(RootComponent);
}

/// <summary>
/// アクター生成・開始時に呼び出される初期化処理。
/// メニューアクターの取得とコールバック設定を行います。
/// </summary>
void AColorBallCharacter::BeginPlay()
{
	Super::BeginPlay();

	// レベル内からEquipActorを検索して取得
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEquipActor::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors) {
		EquipActor = Cast<AEquipActor>(Actor); // メニュー管理アクター
	}
	verifyf(EquipActor != nullptr, TEXT("レベル上にAEquipActorを１つ配置してください。"));

	// メニュー選択時のコールバックを設定
	EquipActor->SetSelectCallback(AEquipActor::EEquipIndex::Muzzle, std::bind(&AColorBallCharacter::SelectMuzzleComponent, this, std::placeholders::_1));
	EquipActor->SetSelectCallback(AEquipActor::EEquipIndex::Repeater, std::bind(&AColorBallCharacter::SelectRepeaterComponent, this, std::placeholders::_1));
	EquipActor->SetSelectCallback(AEquipActor::EEquipIndex::Supplier, std::bind(&AColorBallCharacter::SelectSupplierComponent, this, std::placeholders::_1));
	EquipActor->SetSelectCallback(AEquipActor::EEquipIndex::Bullet, std::bind(&AColorBallCharacter::SelectBulletActor, this, std::placeholders::_1));
	EquipActor->SetSelectCallback(AEquipActor::EEquipIndex::Material, std::bind(&AColorBallCharacter::SelectBulletMaterial, this, std::placeholders::_1));

	// メニューの選択状態をリセット
	EquipActor->ResetCurrentIndices();
}

/// <summary>
/// コントローラーが変更されたときに呼び出される処理。
/// 入力マッピングコンテキストを追加します。
/// </summary>
void AColorBallCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// 入力マッピングコンテキストの追加
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0); // 入力マッピング
		}
	}
}

/// <summary>
/// 入力バインドの設定。
/// 各種アクション（ジャンプ・移動・視点・射撃）を入力にバインドします。
/// </summary>
/// <param name="PlayerInputComponent">入力コンポーネント</param>
void AColorBallCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// アクションバインドの設定
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// ジャンプ
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// 移動
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AColorBallCharacter::Move);

		// 視点
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AColorBallCharacter::Look);

		// 射撃
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AColorBallCharacter::StartFire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AColorBallCharacter::StopFire);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Enhanced Input componentが見つかりません。Enhanced Inputシステム用テンプレートです。レガシーシステムを使う場合はC++ファイルの修正が必要です。"), *GetNameSafe(this));
	}
}

/// <summary>
/// 移動入力処理。
/// 入力値に応じてキャラクターを前後左右に移動させます。
/// </summary>
/// <param name="Value">入力値（Vector2D）</param>
void AColorBallCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// 前方方向取得
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

/// <summary>
/// 視点入力処理。
/// 入力値に応じてカメラのYaw/Pitchを操作します。
/// </summary>
/// <param name="Value">入力値（Vector2D）</param>
void AColorBallCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

/// <summary>
/// 射撃開始処理。
/// 射撃コンポーネントに射撃開始を通知します。
/// </summary>
/// <param name="Value">入力値</param>
void AColorBallCharacter::StartFire(const FInputActionValue& Value)
{
	if (ShooterComponent) {
		ShooterComponent->StartFire();
	}
}

/// <summary>
/// 射撃停止処理。
/// 射撃コンポーネントに射撃停止を通知します。
/// </summary>
/// <param name="Value">入力値</param>
void AColorBallCharacter::StopFire(const FInputActionValue& Value)
{
	if (ShooterComponent) {
		ShooterComponent->StopFire();
	}
}

/// <summary>
/// コンポーネント選択処理。
/// メニューから選択されたコンポーネントをShooterComponentに反映します。
/// </summary>
/// <param name="InAssetPath">選択されたアイテム名</param>
void AColorBallCharacter::SelectMuzzleComponent(const FString& InAssetPath)
{
	if (ShooterComponent) {
		ShooterComponent->ChangeComponent<UMuzzleComponent>(*InAssetPath);
	}
}

/// <summary>
/// コンポーネント選択処理。
/// メニューから選択されたコンポーネントをShooterComponentに反映します。
/// </summary>
/// <param name="InAssetPath">選択されたアイテム名</param>
void AColorBallCharacter::SelectRepeaterComponent(const FString& InAssetPath)
{
	if (ShooterComponent) {
		ShooterComponent->ChangeComponent<URepeaterComponent>(*InAssetPath);
	}
}

/// <summary>
/// コンポーネント選択処理。
/// メニューから選択されたコンポーネントをShooterComponentに反映します。
/// </summary>
/// <param name="InAssetPath">選択されたアイテム名</param>
void AColorBallCharacter::SelectSupplierComponent(const FString& InAssetPath)
{
	if (ShooterComponent) {
		ShooterComponent->ChangeComponent<USupplierComponent>(*InAssetPath);
	}
}

/// <summary>
/// 弾丸アクター選択処理。
/// </summary>
/// <param name="InAssetPath"></param>
void AColorBallCharacter::SelectBulletActor(const FString& InAssetPath)
{
	if (ShooterComponent) {
		ShooterComponent->SetBulletActor(InAssetPath);
	}
}

/// <summary>
/// 弾丸マテリアル選択処理。
/// </summary>
/// <param name="InAssetPath"></param>
void AColorBallCharacter::SelectBulletMaterial(const FString& InAssetPath)
{
	if (ShooterComponent) {
		ShooterComponent->SetBulletMaterial(InAssetPath);
	}
}
