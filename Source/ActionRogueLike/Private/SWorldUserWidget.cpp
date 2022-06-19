// Fill out your copyright notice in the Description page of Project Settings.


#include "SWorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"

void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(AttachedActor))
	{
		RemoveFromParent();

		UE_LOG(LogTemp, Warning, TEXT("AttachedActor nolonger valid, removing health widget!"));
		return;
	}

	FVector2d ScreenPos;
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPos ))
	{
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);

		ScreenPos /= Scale;

		if (ParentSizeBox )
		{
			ParentSizeBox->SetRenderTranslation(ScreenPos);
		}
	}
}
