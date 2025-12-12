// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RepeaterComponent.h"
#include "RapidBurstRepeaterComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLORBALL_API URapidBurstRepeaterComponent : public URepeaterComponent
{
	GENERATED_BODY()

public:

	URapidBurstRepeaterComponent() {
		RapidRate = 0.3f; // 連射間隔（秒）
		BurstCount = 3; // バースト発射数
		BurstRate = 0.05f; // バースト間隔（秒）
	}

	virtual ~URapidBurstRepeaterComponent() {
	}

};
