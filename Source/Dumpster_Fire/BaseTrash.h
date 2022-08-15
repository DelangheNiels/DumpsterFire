// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTrash.generated.h"

UCLASS()
class DUMPSTER_FIRE_API ABaseTrash : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseTrash();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* m_pTrashMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Points", meta =(DisplayPriority = "1"))
		int m_Points{ 1 };

	int GetPoints() const;
	void SetIsThrown(bool isThrown);

	void SetPlayer(AActor* pPlayer);

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "HitBox", meta = (DisplayPriority = "2"))
		class UBoxComponent* m_pCollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitBox", meta = (DisplayPriority = "3"))
		 FVector m_meshSize{53.f,53.f,53.f};

	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	bool m_isThrown{false};

	AActor* m_pPlayer;

};
