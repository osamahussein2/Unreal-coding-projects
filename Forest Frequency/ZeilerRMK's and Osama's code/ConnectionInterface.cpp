// Fill out your copyright notice in the Description page of Project Settings.


#include "ConnectionInterface.h"
#include "PowerCable.h"

// Add default functionality here for any IConnectionInterface functions that are not pure virtual.
bool IConnectionInterface::CanConnectCable_Implementation(APowerCable* Cable) const
{
	return false;
}

void IConnectionInterface::ConnectCable_Implementation(APowerCable* Cable)
{
}

void IConnectionInterface::DisconnectCable_Implementation(APowerCable* Cable)
{
}