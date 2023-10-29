#include "pch.h"
#include "SelectGDI.h"

#include "CCore.h"

SelectGDI::SelectGDI(HDC dc_, PEN_TYPE ePenType_)
	: m_hDC(dc_)
	, m_hDefaultPen(nullptr)
	, m_hDefaultBrush(nullptr)
{
	HPEN hPen = CCore::GetInstance()->GetPen(ePenType_);
	m_hDefaultPen = (HPEN)SelectObject(dc_, hPen);
}

SelectGDI::SelectGDI(HDC dc_, BRUSH_TYPE eBrushType_)
	: m_hDC(dc_)
	, m_hDefaultPen(nullptr)
	, m_hDefaultBrush(nullptr)
{
	HBRUSH hBrush = CCore::GetInstance()->GetBrush(eBrushType_);
	m_hDefaultBrush = (HBRUSH)SelectObject(dc_, hBrush);
}

SelectGDI::~SelectGDI()
{
	SelectObject(m_hDC, m_hDefaultPen);
	SelectObject(m_hDC, m_hDefaultBrush);
}
