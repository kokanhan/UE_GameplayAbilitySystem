// Copyright Moonlit Fantasy Nation


#include "Character/AuraEnemy.h"
#include "Aura/Aura.h"
#include "DrawDebugHelpers.h"
#include <AbilitySystem/AuraAbilitySystemComponent.h>
#include <AbilitySystem/AuraAttributeSet.h>
//#include "Kismet/GameplayStatics.h" // For UGameplayStatics
AAuraEnemy::AAuraEnemy()
{
    PrimaryActorTick.bCanEverTick = true;
    //use code to auto-set BPs collision presets, no manual hard work:)
    //���Ǵ�������ȼ��ǵ���BP���õģ�Ҫȷ����һ��
    GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);

    AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

    AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSte");
}
void AAuraEnemy::HighLightActor()
{
    //bHighlighted = true; ����Ҫ���boolean��
    GetMesh()->SetRenderCustomDepth(true);
    GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);//����һ�ξͿ����� �������ﱻ���úܶ�� ��Ϊ��������ʱ��Ӱ��performance

    Weapon->SetRenderCustomDepth(true);
    Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighLightActor()
{
    //bHighlighted = false;
    GetMesh()->SetRenderCustomDepth(false);

    Weapon->SetRenderCustomDepth(false);
}

void AAuraEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //use post-process highlight instead
    /*if (bHighlighted)
    {
        FVector SphereCenter = GetActorLocation();
        float SphereRadius = 50.0f;
        FColor SphereColor = FColor::Red;
        DrawDebugSphere(
            GetWorld(),
            SphereCenter,
            SphereRadius,
            12,
            SphereColor,
            false,
            0.0f,
            0,
            1.0f
        );
    }*/
}

void AAuraEnemy::BeginPlay()
{
    Super::BeginPlay();
    check(AbilitySystemComponent);//Ҳ���Բ�д ���AbilitySystemComponent��nullprt��AAuraEnemy constructor��ᱨ��
    AbilitySystemComponent->InitAbilityActorInfo(this, this);  // AI��ɫ�������ߺͱ�ʾ�߶����Լ�

}
