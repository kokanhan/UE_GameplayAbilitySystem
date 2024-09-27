// Copyright Moonlit Fantasy Nation

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();
	virtual void Tick(float DeltaTime) override;

	/** Enemy Interface **/
	virtual void HighLightActor() override;
	virtual void UnHighLightActor() override;
	// no need for this boolean, use post-process instead
	/*UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;*/
	/** End Enemy Interface **/

protected:
	virtual void BeginPlay() override;

};
