#include"pch.h"
#include"InputDelegator.h"

void IInputDelegator::AttachHandler(IInputHandler* pHandler)
{
    m_listHandlers.push_back(pHandler);
    pHandler->AssignDelegator(this);
}

void IInputDelegator::DetachHandler(IInputHandler* pHandler)
{
    m_listHandlers.remove(pHandler);
    pHandler->AssignDelegator(nullptr);
}