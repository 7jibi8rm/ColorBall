// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SupplierComponent.generated.h"

/**
 * <summary>
 * 給弾装置（Supplier）の基本コンポーネントクラス。
 * 発射口の管理や、発射口インデックスの制御を行います。
 * 継承して様々な給弾装置の挙動を実装できます。
 * </summary>
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COLORBALL_API USupplierComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    /// <summary>
    /// コンストラクタ。
    /// コンポーネントのデフォルト値を設定します。
    /// </summary>
    USupplierComponent();

    /// <summary>
    /// 給弾装置を使用可能な状態に設定します。
    /// 発射口数を指定し、インデックス配列を初期化します。
    /// </summary>
    /// <param name="InMuzzleCount">発射口の総数</param>
    void SetupSpplier(const int32 InMuzzleCount);

    /// <summary>
    /// 発射口インデックス配列を取得します。
    /// 現在有効な発射口のインデックス一覧を返します。
    /// </summary>
    /// <returns>発射口インデックス配列（const参照）</returns>
    const TArray<int32>& GetFireMuzzleIndexes() const;

    /// <summary>
    /// 次の発射口インデックスを計算します。
    /// 派生クラスでオーバーライドすることで、発射順序の制御が可能です。
    /// </summary>
    virtual void NextFireMuzzle();

protected:
    // 発射口数（砲台の総数）
    int32 MuzzleCount{ 0 };

    // 発射口インデックス配列（次に発射する砲台のインデックス一覧）
    TArray<int32> FireMuzzleIndexes{};

    // 現在の発射口インデックス（発射順序管理用）
    int32 CurrentFireMuzzleIndex{ 0 };
};