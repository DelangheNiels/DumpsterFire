// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dumpster_Fire/BaseTrash.h"
#include "JerryCan.generated.h"

/**
 * 
 */
UCLASS()
class DUMPSTER_FIRE_API AJerryCan : public ABaseTrash
{
	GENERATED_BODY()

public:
	AJerryCan();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
