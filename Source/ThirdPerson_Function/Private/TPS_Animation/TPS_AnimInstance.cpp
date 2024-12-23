// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Animation/TPS_AnimInstance.h"

void UTPS_AnimInstance::AnimNotify_Draw()
{
	DrawCheck.Broadcast();
}
