// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/KismetSystemLibrary.h"
#include "ShooterComponent.h"
#include "Repeater/RepeaterComponent.h"
#include "Supplier/SupplierComponent.h"
#include "Muzzle/SpreaderMuzzleComponent.h"

/// <summary>
/// コンストラクタ。
/// ShooterComponentの初期化処理を行います。
/// </summary>
UShooterComponent::UShooterComponent()
{
	// ゲーム開始時に初期化し、毎フレームTickを有効化
	PrimaryComponentTick.bCanEverTick = true;
}

/// <summary>
/// 弾薬クラスを設定します。
/// 武器から発射される弾薬の種類を指定します。
/// </summary>
/// <param name="InBulletClass">設定する弾薬クラス</param>
void UShooterComponent::SetBulletActor(const FString& InAssetPath)
{
	BulletClass = LoadObject<UClass>(nullptr, *InAssetPath);
}

/// <summary>
/// 弾薬クラスを設定します。
/// 武器から発射される弾薬の種類を指定します。
/// </summary>
/// <param name="InBulletClass">設定する弾薬クラス</param>
void UShooterComponent::SetBulletMaterial(const FString& InAssetPath)
{
	SelectedMaterial = LoadObject<UMaterialInterface>(nullptr, *InAssetPath);
}

/// <summary>
/// 射撃開始処理。
/// 砲台・給弾装置・火器管制の初期化を行い、射撃サイクルを開始します。
/// </summary>
void UShooterComponent::StartFire()
{
	// 砲台起動。砲台数を取得
	int32 MuzzleCount = 0;
	UMuzzleComponent* Muzzle = FindComponent<UMuzzleComponent>();
	if (Muzzle != nullptr) {
		Muzzle->CalcMuzzleTransforms(ElapsedTimeCounter);
		MuzzleCount = Muzzle->GetMuzzleTransforms().Num(); // 砲台数取得
	}
	// 給弾装置起動。砲台数を渡す
	USupplierComponent* Supplier = FindComponent<USupplierComponent>();
	if (Supplier != nullptr) {
		Supplier->SetupSpplier(MuzzleCount);
	}
	// 火器管制システム起動。射撃実行関数を登録
	URepeaterComponent* Repeater = FindComponent<URepeaterComponent>();
	if (Repeater != nullptr) {
		Repeater->StartRepeater(std::bind(&UShooterComponent::ExecuteFire, this), MuzzleCount);
	}
}

/// <summary>
/// 射撃停止処理。
/// 火器管制システムの射撃サイクルを停止します。
/// </summary>
void UShooterComponent::StopFire()
{
	URepeaterComponent* Repeater = FindComponent<URepeaterComponent>();
	if (Repeater != nullptr) {
		Repeater->StopRepeater();
	}
}

/// <summary>
/// 射撃実行処理。
/// 砲台・給弾装置の情報を元に弾薬をスポーンします。
/// </summary>
void UShooterComponent::ExecuteFire()
{
	UMuzzleComponent* Muzzle = FindComponent<UMuzzleComponent>();
	if (Muzzle == nullptr) {
		return;
	}
	USupplierComponent* Supplier = FindComponent<USupplierComponent>();
	if (Supplier == nullptr) {
		return;
	}
	if (SelectedMaterial == nullptr) {
		return;
	}

	// 砲台位置を計算
	Muzzle->CalcMuzzleTransforms(ElapsedTimeCounter);

	// 次の発射口を計算
	Supplier->NextFireMuzzle();
	const TArray<int32>& FireMuzzleIndexes = Supplier->GetFireMuzzleIndexes();

	// 弾薬スポーン用パラメータ設定
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = GetOwner()->GetInstigator();

	// 砲台位置から弾薬を発射
	for (const auto& FireMuzzleIndex : FireMuzzleIndexes)
	{
		if (FireMuzzleIndex < 0 || FireMuzzleIndex >= Muzzle->GetMuzzleTransforms().Num()) {
			verifyf(false, TEXT("発射口番号がおかしい: %d / %d"), FireMuzzleIndex, Muzzle->GetMuzzleTransforms().Num());
			continue;
		}
		const FTransform& ShooterTransform = Muzzle->GetMuzzleTransforms()[FireMuzzleIndex];
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(BulletClass, ShooterTransform.GetLocation(), ShooterTransform.Rotator(), SpawnParams);
		if (SpawnedActor)
		{
			UStaticMeshComponent* MeshComp = SpawnedActor->FindComponentByClass<UStaticMeshComponent>();
			if (MeshComp)
			{
				MeshComp->SetMaterial(0, SelectedMaterial);  // スロット0に設定
			}
		}
	}
}

/// <summary>
/// 毎フレーム呼び出されるTick処理。
/// 経過時間の更新や砲台位置のデバッグ表示を行います。
/// </summary>
/// <param name="DeltaTime">前フレームからの経過時間</param>
/// <param name="TickType">Tickの種類</param>
/// <param name="ThisTickFunction">Tick関数情報</param>
void UShooterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 経過時間カウンター更新
	ElapsedTimeCounter += DeltaTime;

	// デバッグ用：砲台位置を表示
	UMuzzleComponent* Muzzle = FindComponent<UMuzzleComponent>();
	if (Muzzle != nullptr) {
		Muzzle->CalcMuzzleTransforms(ElapsedTimeCounter);
		for (const auto& Transforms : Muzzle->GetMuzzleTransforms()) {
			const FVector StartLocation = Transforms.GetLocation();
			const FVector EndLocation = StartLocation + (Transforms.Rotator().Vector() * 250.0f);
			DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, 0.0f, 0, 1.0f);
		}
	}
}
