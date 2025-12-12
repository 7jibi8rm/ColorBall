// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "RepeaterComponent.generated.h"

/**
 * <summary>
 * リピーター（連射・バースト射撃）制御用の基本コンポーネント。
 * 連射間隔やバースト数などのパラメータ管理、射撃コールバックの制御を行います。
 * 派生クラスで様々なリピーター挙動を実装可能です。
 * </summary>
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COLORBALL_API URepeaterComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    /// <summary>
    /// コンストラクタ。
    /// コンポーネントのデフォルト値を設定します。
    /// </summary>
    URepeaterComponent();

    /// <summary>
    /// リピーター開始処理。
    /// 連射・バースト射撃を開始し、指定されたコールバック関数を射撃時に呼び出します。
    /// </summary>
    /// <param name="InFireCallback">射撃時に呼び出すコールバック関数</param>
    void StartRepeater(TFunction<void()> InFireCallback, int32 InMuzzle);

    /// <summary>
    /// リピーター停止処理。
    /// 連射・バースト射撃を停止します。
    /// </summary>
    void StopRepeater();

protected:

    /// <summary>
    /// リピーター開始時の処理。
    /// 派生クラスでオーバーライドし、開始時のカスタム処理を実装可能です。
    /// </summary>
    virtual void OnStartRepeater();

    // 連射間隔（秒）
    float RapidRate{ 9999.0f };

    // バースト発射数
    int32 BurstCount{ 1 };

    // バースト間隔（秒）
    float BurstRate{ 0.1f };

    // 砲台数
    int32 MuzzleCount{ 0 };

private:
    /// <summary>
    /// 連射発射処理。
    /// RapidRateに従い、射撃コールバックを定期的に呼び出します。
    /// </summary>
    void HandleRapidFire();

    /// <summary>
    /// バースト発射処理。
    /// BurstCount/BurstRateに従い、複数回の射撃コールバックを制御します。
    /// </summary>
    void HandleBurstFire();

    // 連射用タイマー
    FTimerHandle RapidFireTimer{};

    // バースト用タイマー
    FTimerHandle BurstFireTimer{};

    // 現在のバースト発射数
    int32 CurrentBurstCount{ 0 };

    // 射撃時コールバック関数
    TFunction<void()> FireCallback = {};
};