// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupableObject.h"

// Sets default values
APickupableObject::APickupableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APickupableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

