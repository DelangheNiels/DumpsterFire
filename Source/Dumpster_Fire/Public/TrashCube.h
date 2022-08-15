// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseTrash.h"
#include "GameFramework/Actor.h"
#include "TrashCube.generated.h"


UCLASS()
class DUMPSTER_FIRE_API ATrashCube : public ABaseTrash
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrashCube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
