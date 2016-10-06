////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      MaterialCompiler
//  File name:   Main.cpp
//  Created:     2016/08/09 by Albert
//  Description:
// -------------------------------------------------------------------------
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
// -------------------------------------------------------------------------
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
// -------------------------------------------------------------------------
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////

#include "Material.h"

VE_PARSER_ENUM(D3D12_ROOT_PARAMETER_TYPE)
{
	VE_PARSER_ENUM_ITEM("table", D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE);
	VE_PARSER_ENUM_ITEM("const", D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS);
	VE_PARSER_ENUM_ITEM("cbv", D3D12_ROOT_PARAMETER_TYPE_CBV);
	VE_PARSER_ENUM_ITEM("srv", D3D12_ROOT_PARAMETER_TYPE_SRV);
	VE_PARSER_ENUM_ITEM("uav", D3D12_ROOT_PARAMETER_TYPE_UAV);
}

VE_PARSER_ENUM(D3D12_SHADER_VISIBILITY)
{
	VE_PARSER_ENUM_ITEM("all", D3D12_SHADER_VISIBILITY_ALL);
	VE_PARSER_ENUM_ITEM("vs", D3D12_SHADER_VISIBILITY_VERTEX);
	VE_PARSER_ENUM_ITEM("hs", D3D12_SHADER_VISIBILITY_HULL);
	VE_PARSER_ENUM_ITEM("ds", D3D12_SHADER_VISIBILITY_DOMAIN);
	VE_PARSER_ENUM_ITEM("gs", D3D12_SHADER_VISIBILITY_GEOMETRY);
	VE_PARSER_ENUM_ITEM("ps", D3D12_SHADER_VISIBILITY_PIXEL);
}

VE_PARSER_ENUM(D3D12_DESCRIPTOR_RANGE_TYPE)
{
	VE_PARSER_ENUM_ITEM("srv", D3D12_DESCRIPTOR_RANGE_TYPE_SRV);
	VE_PARSER_ENUM_ITEM("uav", D3D12_DESCRIPTOR_RANGE_TYPE_UAV);
	VE_PARSER_ENUM_ITEM("cbv", D3D12_DESCRIPTOR_RANGE_TYPE_CBV);
	VE_PARSER_ENUM_ITEM("sampler", D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER);
}

VE_PARSER_ENUM(RootDescriptorLevel)
{
	VE_PARSER_ENUM_ITEM("static", LEVEL_STATIC);
	VE_PARSER_ENUM_ITEM("frame", LEVEL_FRAME_STATIC);
	VE_PARSER_ENUM_ITEM("dynamic", LEVEL_DYNAMIC);
}

VE_PARSER_ENUM(D3D12_FILTER)
{
	VE_PARSER_ENUM_ITEM("min_mag_mip_point", D3D12_FILTER_MIN_MAG_MIP_POINT);
	VE_PARSER_ENUM_ITEM("min_mag_point_mip_linear", D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR);
	VE_PARSER_ENUM_ITEM("min_point_mag_linear_mip_point", D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT);
	VE_PARSER_ENUM_ITEM("min_point_mag_mip_linear", D3D12_FILTER_MIN_POINT_MAG_MIP_LINEAR);
	VE_PARSER_ENUM_ITEM("min_linear_mag_mip_point", D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT);
	VE_PARSER_ENUM_ITEM("min_linear_mag_point_mip_linear", D3D12_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR);
	VE_PARSER_ENUM_ITEM("min_mag_linear_mip_point", D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT);
	VE_PARSER_ENUM_ITEM("min_mag_mip_linear", D3D12_FILTER_MIN_MAG_MIP_LINEAR);
	VE_PARSER_ENUM_ITEM("anisotropic", D3D12_FILTER_ANISOTROPIC);
}

VE_PARSER_ENUM(D3D12_TEXTURE_ADDRESS_MODE)
{
	VE_PARSER_ENUM_ITEM("wrap", D3D12_TEXTURE_ADDRESS_MODE_WRAP);
	VE_PARSER_ENUM_ITEM("mirror", D3D12_TEXTURE_ADDRESS_MODE_MIRROR);
	VE_PARSER_ENUM_ITEM("clamp", D3D12_TEXTURE_ADDRESS_MODE_CLAMP);
	VE_PARSER_ENUM_ITEM("border", D3D12_TEXTURE_ADDRESS_MODE_BORDER);
	VE_PARSER_ENUM_ITEM("mirror_once", D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE);
}

VE_PARSER_ENUM(D3D12_COMPARISON_FUNC)
{
	VE_PARSER_ENUM_ITEM("never", D3D12_COMPARISON_FUNC_NEVER);
	VE_PARSER_ENUM_ITEM("lt", D3D12_COMPARISON_FUNC_LESS);
	VE_PARSER_ENUM_ITEM("eq", D3D12_COMPARISON_FUNC_EQUAL);
	VE_PARSER_ENUM_ITEM("le", D3D12_COMPARISON_FUNC_LESS_EQUAL);
	VE_PARSER_ENUM_ITEM("gt", D3D12_COMPARISON_FUNC_GREATER);
	VE_PARSER_ENUM_ITEM("ne", D3D12_COMPARISON_FUNC_NOT_EQUAL);
	VE_PARSER_ENUM_ITEM("ge", D3D12_COMPARISON_FUNC_GREATER_EQUAL);
	VE_PARSER_ENUM_ITEM("always", D3D12_COMPARISON_FUNC_ALWAYS);
}

VE_PARSER_ENUM(D3D12_STATIC_BORDER_COLOR)
{
	VE_PARSER_ENUM_ITEM("transparent", D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK);
	VE_PARSER_ENUM_ITEM("op_black", D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK);
	VE_PARSER_ENUM_ITEM("op_white", D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE);
}

int main(int argc, char** argv)
{
	VE_ASSERT_ALWAYS(argc == 3);
	VeInitData kData =
	{
		"MaterialCompiler",
		VE_MAKE_VERSION(0, 1),
		VE_INIT_CONSOLE,
		nullptr
	};
	VeInit(kData);
	{
		Material kMaterial(argv[1]);
		if (kMaterial.Init(argv[2]))
		{
			kMaterial.Compile();
		}
	}
	VeTerm();
	return 0;
}
