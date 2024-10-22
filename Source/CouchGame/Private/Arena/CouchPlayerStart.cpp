// Fill out your copyright notice in the Description page of Project Settings.


#include "CouchGame/Public/Arena/CouchPlayerStart.h"

float ACouchPlayerStart::GetStartOrientX() const
{
	switch (StartOrient)
	{
	case ECouchPlayerStartOrient::Left: return -1.f;
	case ECouchPlayerStartOrient::Right: return 1.f;
	}

	return 0.f;
}
