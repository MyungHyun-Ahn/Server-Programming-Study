#pragma once
class SelectGDI
{
public:
	SelectGDI(HDC dc_, PEN_TYPE ePenType_);
	SelectGDI(HDC dc_, BRUSH_TYPE eBrushType_);
	~SelectGDI();

private:
	HDC		m_hDC;
	HPEN	m_hDefaultPen;
	HBRUSH	m_hDefaultBrush;
};

