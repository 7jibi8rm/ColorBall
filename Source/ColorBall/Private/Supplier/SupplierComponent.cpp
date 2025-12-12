// Fill out your copyright notice in the Description page of Project Settings.

#include "Supplier/SupplierComponent.h"

/// <summary>
/// コンストラクタ。
/// コンポーネントのデフォルト値を設定し、毎フレームTickを有効化します。
/// </summary>
USupplierComponent::USupplierComponent()
{
	// ゲーム開始時に初期化し、毎フレームTickを有効化
	PrimaryComponentTick.bCanEverTick = true;
}

/// <summary>
/// 給弾装置を使用可能な状態に設定します。
/// 発射口数を指定し、インデックス配列を初期化します。
/// </summary>
/// <param name="InMuzzleCount">発射口の総数</param>
void USupplierComponent::SetupSpplier(const int32 InMuzzleCount)
{
	MuzzleCount = InMuzzleCount; // 発射口数（砲台の総数）
}

/// <summary>
/// 発射口インデックス配列を取得します。
/// 現在有効な発射口のインデックス一覧を返します。
/// </summary>
/// <returns>発射口インデックス配列（const参照）</returns>
const TArray<int32>& USupplierComponent::GetFireMuzzleIndexes() const
{
	return FireMuzzleIndexes; // 発射口インデックス配列
}

/// <summary>
/// 次の発射口インデックスを計算します。
/// 基底クラスは全発射口を対象とします。
/// 派生クラスでオーバーライドすることで、発射順序の制御が可能です。
/// </summary>
void USupplierComponent::NextFireMuzzle()
{
	// 全発射口を対象にインデックス配列を更新
	FireMuzzleIndexes.Empty(); // 発射口インデックス配列（次に発射する砲台のインデックス一覧）
	for (int32 Index = 0; Index < MuzzleCount; ++Index) {
		FireMuzzleIndexes.Add(Index);
	}
}