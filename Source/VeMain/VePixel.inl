////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VePixel.inl
//  Created:     2015/08/07 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline VeColor::operator VeRGBA () const noexcept
{
	VeRGBA kRes;
	GetAs(kRes);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeColor::operator VeFloat32* () noexcept
{
	return VE_FLOAT_POINT_THIS;
}
//--------------------------------------------------------------------------
inline VeColor::operator const VeFloat32* () const noexcept
{
	return VE_FLOAT_POINT_THIS;
}
//--------------------------------------------------------------------------
inline VeColor& VeColor::operator = (VeFloat32 f32Scalar) noexcept
{
	VeSetv4f(VE_FLOAT_POINT_THIS, f32Scalar);
	return *this;
}
//--------------------------------------------------------------------------
inline bool VeColor::operator == (const VeColor& c) const noexcept
{
	return VeEqualv4f(VE_FLOAT_POINT_THIS, c);
}
//--------------------------------------------------------------------------
inline bool VeColor::operator != (const VeColor& c) const noexcept
{
	return !VeEqualv4f(VE_FLOAT_POINT_THIS, c);
}
//--------------------------------------------------------------------------
inline VeColor VeColor::operator + (const VeColor& c) const noexcept
{
	VeColor kRes;
	VeAddv4f(kRes, VE_FLOAT_POINT_THIS, c);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeColor VeColor::operator - (const VeColor& c) const noexcept
{
	VeColor kRes;
	VeSubv4f(kRes, VE_FLOAT_POINT_THIS, c);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeColor VeColor::operator * (VeFloat32 f32Scalar) const noexcept
{
	VeColor kRes;
	VeMulv4f(kRes, VE_FLOAT_POINT_THIS, f32Scalar);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeColor VeColor::operator * (const VeColor& c) const noexcept
{
	VeColor kRes;
	VeMulv4f(kRes, VE_FLOAT_POINT_THIS, c);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeColor VeColor::operator / (VeFloat32 f32Scalar) const noexcept
{
	VeColor kRes;
	VeDivv4f(kRes, VE_FLOAT_POINT_THIS, f32Scalar);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeColor VeColor::operator / (const VeColor& c) const noexcept
{
	VeColor kRes;
	VeDivv4f(kRes, VE_FLOAT_POINT_THIS, c);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeColor VeColor::operator - () const noexcept
{
	return VeColor(-r, -g, -b, -a);
}
//--------------------------------------------------------------------------
inline VeColor operator * (VeFloat32 f32Scalar, const VeColor& c) noexcept
{
	return c * f32Scalar;
}
//--------------------------------------------------------------------------
inline VeColor& VeColor::operator += (const VeColor& c) noexcept
{
	VeAddv4f(VE_FLOAT_POINT_THIS, VE_FLOAT_POINT_THIS, c);
	return *this;
}
//--------------------------------------------------------------------------
inline VeColor& VeColor::operator -= (const VeColor& c) noexcept
{
	VeSubv4f(VE_FLOAT_POINT_THIS, VE_FLOAT_POINT_THIS, c);
	return *this;
}
//--------------------------------------------------------------------------
inline VeColor& VeColor::operator *= (VeFloat32 f32Scalar) noexcept
{
	VeMulv4f(VE_FLOAT_POINT_THIS, VE_FLOAT_POINT_THIS, f32Scalar);
	return *this;
}
//--------------------------------------------------------------------------
inline VeColor& VeColor::operator *= (const VeColor& c) noexcept
{
	VeMulv4f(VE_FLOAT_POINT_THIS, VE_FLOAT_POINT_THIS, c);
	return *this;
}
//--------------------------------------------------------------------------
inline VeColor& VeColor::operator /= (VeFloat32 f32Scalar) noexcept
{
	VeDivv4f(VE_FLOAT_POINT_THIS, VE_FLOAT_POINT_THIS, f32Scalar);
	return *this;
}
//--------------------------------------------------------------------------
inline VeColor& VeColor::operator /= (const VeColor& c) noexcept
{
	VeDivv4f(VE_FLOAT_POINT_THIS, VE_FLOAT_POINT_THIS, c);
	return *this;
}
//--------------------------------------------------------------------------
inline void VeColor::Clamp() noexcept
{
	VeClampv4f(VE_FLOAT_POINT_THIS, VE_FLOAT_POINT_THIS);
}
//--------------------------------------------------------------------------
inline void VeColor::Scale() noexcept
{
	VeFloat32 f32Temp = VE_MAX(VE_MAX(r, g), b);
	if (f32Temp > 1.0f)
	{
		f32Temp = 1.0f / f32Temp;
		r *= f32Temp;
		g *= f32Temp;
		b *= f32Temp;
	}

	if (a > 1.0f)
		a = 1.0f;
}
//--------------------------------------------------------------------------
inline void VeColor::GetAs(VeRGBA& kColor) const noexcept
{
	kColor.r() = (VeUInt8)(VeSat(r) * 255.0f);
	kColor.g() = (VeUInt8)(VeSat(g) * 255.0f);
	kColor.b() = (VeUInt8)(VeSat(b) * 255.0f);
	kColor.a() = (VeUInt8)(VeSat(a) * 255.0f);
}
//--------------------------------------------------------------------------
inline VeRGBA::operator VeColor () noexcept
{
	VeColor kRes;
	GetAs(kRes);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeUInt8& VeRGBA::r() noexcept
{
	return m_kColor.r;
}
//--------------------------------------------------------------------------
inline const VeUInt8& VeRGBA::r() const noexcept
{
	return m_kColor.r;
}
//--------------------------------------------------------------------------
inline VeUInt8& VeRGBA::g() noexcept
{
	return m_kColor.g;
}
//--------------------------------------------------------------------------
inline const VeUInt8& VeRGBA::g() const noexcept
{
	return m_kColor.g;
}
//--------------------------------------------------------------------------
inline VeUInt8& VeRGBA::b() noexcept
{
	return m_kColor.b;
}
//--------------------------------------------------------------------------
inline const VeUInt8& VeRGBA::b() const noexcept
{
	return m_kColor.b;
}
//--------------------------------------------------------------------------
inline VeUInt8& VeRGBA::a() noexcept
{
	return m_kColor.a;
}
//--------------------------------------------------------------------------
inline const VeUInt8& VeRGBA::a() const noexcept
{
	return m_kColor.a;
}
//--------------------------------------------------------------------------
inline VeRGBA& VeRGBA::operator = (VeUInt8 uiValue) noexcept
{
	r() = uiValue;
	g() = uiValue;
	b() = uiValue;
	a() = uiValue;
	return *this;
}
//--------------------------------------------------------------------------
inline bool VeRGBA::operator == (const VeRGBA& c) const noexcept
{
	return (r() == c.r() && g() == c.g() && b() == c.b() && a() == c.a());
}
//--------------------------------------------------------------------------
inline bool VeRGBA::operator!= (const VeRGBA& c) const noexcept
{
	return !(*this == c);
}
//--------------------------------------------------------------------------
inline VeRGBA VeRGBA::operator + (const VeRGBA& c) const noexcept
{
	VeRGBA kRes;
	kRes.r() = (VeUInt8)VE_MIN(255, r() + c.r());
	kRes.g() = (VeUInt8)VE_MIN(255, g() + c.g());
	kRes.b() = (VeUInt8)VE_MIN(255, b() + c.b());
	kRes.a() = (VeUInt8)VE_MIN(255, a() + c.a());
	return kRes;
}
//--------------------------------------------------------------------------
inline VeRGBA VeRGBA::operator - (const VeRGBA& c) const noexcept
{
	VeRGBA kRes;
	kRes.r() = (VeUInt8)VE_MAX(0, r() - c.r());
	kRes.g() = (VeUInt8)VE_MAX(0, g() - c.g());
	kRes.b() = (VeUInt8)VE_MAX(0, b() - c.b());
	kRes.a() = (VeUInt8)VE_MAX(0, a() - c.a());
	return kRes;
}
//--------------------------------------------------------------------------
inline VeRGBA VeRGBA::operator * (VeFloat32 f32Scalar) const noexcept
{
	VeRGBA kRes;
	kRes.r() = (VeUInt8)VE_MIN(255.0f, (VeFloat32)r() * f32Scalar);
	kRes.g() = (VeUInt8)VE_MIN(255.0f, (VeFloat32)g() * f32Scalar);
	kRes.b() = (VeUInt8)VE_MIN(255.0f, (VeFloat32)b() * f32Scalar);
	kRes.a() = (VeUInt8)VE_MIN(255.0f, (VeFloat32)a() * f32Scalar);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeRGBA VeRGBA::operator * (const VeRGBA& c) const noexcept
{
	VeRGBA kRes;
	kRes.r() = (VeUInt8)VE_MIN(255, r() * c.r());
	kRes.g() = (VeUInt8)VE_MIN(255, g() * c.g());
	kRes.b() = (VeUInt8)VE_MIN(255, b() * c.b());
	kRes.a() = (VeUInt8)VE_MIN(255, a() * c.a());
	return kRes;
}
//--------------------------------------------------------------------------
inline VeRGBA VeRGBA::operator / (VeFloat32 f32Scalar) const noexcept
{
	VeFloat32 f32InvScalar = 1.0f / f32Scalar;
	VeRGBA kRes;
	kRes.r() = (VeUInt8)VE_MIN(255.0f, (VeFloat32)r() * f32InvScalar);
	kRes.g() = (VeUInt8)VE_MIN(255.0f, (VeFloat32)g() * f32InvScalar);
	kRes.b() = (VeUInt8)VE_MIN(255.0f, (VeFloat32)b() * f32InvScalar);
	kRes.a() = (VeUInt8)VE_MIN(255.0f, (VeFloat32)a() * f32InvScalar);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeRGBA VeRGBA::operator / (const VeRGBA& c) const noexcept
{
	VeRGBA kResult;
	kResult.r() = r() / c.r();
	kResult.g() = g() / c.g();
	kResult.b() = b() / c.b();
	kResult.a() = a() / c.a();
	return kResult;
}
//--------------------------------------------------------------------------
inline VeRGBA VeRGBA::operator - () const noexcept
{
	return VeRGBA(-r(), -g(), -b(), -a());
}
//--------------------------------------------------------------------------
inline VeRGBA operator * (VeFloat32 f32Scalar, const VeRGBA& c) noexcept
{
	return c * f32Scalar;
}
//--------------------------------------------------------------------------
inline VeRGBA& VeRGBA::operator += (const VeRGBA& c) noexcept
{
	r() = (VeUInt8)VE_MIN(255, r() + c.r());
	g() = (VeUInt8)VE_MIN(255, g() + c.g());
	b() = (VeUInt8)VE_MIN(255, b() + c.b());
	a() = (VeUInt8)VE_MIN(255, a() + c.a());
	return *this;
}
//--------------------------------------------------------------------------
inline VeRGBA& VeRGBA::operator -= (const VeRGBA& c) noexcept
{
	r() = (VeUInt8)VE_MAX(0, r() - c.r());
	g() = (VeUInt8)VE_MAX(0, g() - c.g());
	b() = (VeUInt8)VE_MAX(0, b() - c.b());
	a() = (VeUInt8)VE_MAX(0, a() - c.a());
	return *this;
}
//--------------------------------------------------------------------------
inline VeRGBA& VeRGBA::operator *= (VeFloat32 f32Scalar) noexcept
{
	r() = (VeUInt8)VE_MIN(255.0f, (VeFloat32)r() * f32Scalar);
	g() = (VeUInt8)VE_MIN(255.0f, (VeFloat32)g() * f32Scalar);
	b() = (VeUInt8)VE_MIN(255.0f, (VeFloat32)b() * f32Scalar);
	a() = (VeUInt8)VE_MIN(255.0f, (VeFloat32)a() * f32Scalar);
	return *this;
}
//--------------------------------------------------------------------------
inline VeRGBA& VeRGBA::operator *= (const VeRGBA& c) noexcept
{
	r() = (VeUInt8)VE_MIN(255, r() * c.r());
	g() = (VeUInt8)VE_MIN(255, g() * c.g());
	b() = (VeUInt8)VE_MIN(255, b() * c.b());
	a() = (VeUInt8)VE_MIN(255, a() * c.a());
	return *this;
}
//--------------------------------------------------------------------------
inline VeRGBA& VeRGBA::operator /= (VeFloat32 f32Scalar) noexcept
{
	VeFloat32 f32InvScalar = 1.0f / f32Scalar;
	r() = (VeUInt8)VE_MIN(255.0f, (VeFloat32)r() * f32InvScalar);
	g() = (VeUInt8)VE_MIN(255.0f, (VeFloat32)g() * f32InvScalar);
	b() = (VeUInt8)VE_MIN(255.0f, (VeFloat32)b() * f32InvScalar);
	a() = (VeUInt8)VE_MIN(255.0f, (VeFloat32)a() * f32InvScalar);
	return *this;
}
//--------------------------------------------------------------------------
inline VeRGBA& VeRGBA::operator /= (const VeRGBA& c) noexcept
{
	r() = (VeUInt8)(r() / c.r());
	g() = (VeUInt8)(g() / c.g());
	b() = (VeUInt8)(b() / c.b());
	a() = (VeUInt8)(a() / c.a());
	return *this;
}
//--------------------------------------------------------------------------
inline void VeRGBA::GetAs(VeUInt32& u32Color) const noexcept
{
	u32Color = m_u32Color;
}
//--------------------------------------------------------------------------
inline void VeRGBA::GetAsBGRA(VeUInt32& u32Color) const noexcept
{
	VeRGBA kRes(b(), g(), r(), a());
	u32Color = kRes.m_u32Color;
}
//--------------------------------------------------------------------------
inline void VeRGBA::GetAs(VeColor& kColor) const noexcept
{
	kColor.r = ((VeFloat32)r() / 255.0f);
	kColor.g = ((VeFloat32)g() / 255.0f);
	kColor.b = ((VeFloat32)b() / 255.0f);
	kColor.a = ((VeFloat32)a() / 255.0f);
}
//--------------------------------------------------------------------------
inline bool VePixelFormat::SetPalette(
	const VePalettePtr& spPalette) noexcept
{
	if (spPalette && spPalette->m_kColors.size() != (VeSizeT)(1 << m_u8BitsPerPixel))
	{
		return false;
	}

	m_spPalette = spPalette;
	return true;
}
//--------------------------------------------------------------------------
