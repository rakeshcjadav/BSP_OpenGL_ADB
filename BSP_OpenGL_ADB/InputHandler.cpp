#include"pch.h"
#include"InputHandler.h"

void IInputHandler::AssignDelegator(IInputDelegator* pDelegator)
{
    m_pDelegator = pDelegator;
}