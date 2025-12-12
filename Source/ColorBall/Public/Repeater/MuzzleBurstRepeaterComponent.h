// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RepeaterComponent.h"
#include "MuzzleBurstRepeaterComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLORBALL_API UMuzzleBurstRepeaterComponent : public URepeaterComponent
{
	GENERATED_BODY()

public:

	UMuzzleBurstRepeaterComponent() {
	}

	virtual ~UMuzzleBurstRepeaterComponent() {
	}

protected:

	/// <summary>
	/// リピーター開始時の処理。
	/// 派生クラスでオーバーライドし、開始時のカスタム処理を実装可能です。
	/// </summary>
	void OnStartRepeater() override
	{
		RapidRate = 0.3f; // 連射間隔（秒）
		BurstCount = MuzzleCount; // バースト発射数
		BurstRate = 0.05f; // バースト間隔（秒）
    }

	

};
