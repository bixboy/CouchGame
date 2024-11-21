// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileEffect/Effects/CouchProjectileEffectTripleBullets.h"


// Sets default values
ACouchProjectileEffectTripleBullets::ACouchProjectileEffectTripleBullets()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ExecuteTime = ECouchProjectileExecuteTime::OnLaunch;
}



