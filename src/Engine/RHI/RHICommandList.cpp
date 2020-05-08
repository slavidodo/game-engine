
#include "pch.h"

#include "RHICommandList.h"

RHICommandListExecutor gRHICommandList;

RHICommandListImmediate& RHICommandListExecutor::GetImmediateCommandList()
{
	return gRHICommandList.ImmediateCommandList;
}