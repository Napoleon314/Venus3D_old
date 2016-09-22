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
	VE_PARSER_ENUM_ITEM("32bit", D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS);
	VE_PARSER_ENUM_ITEM("cbv", D3D12_ROOT_PARAMETER_TYPE_CBV);
	VE_PARSER_ENUM_ITEM("srv", D3D12_ROOT_PARAMETER_TYPE_SRV);
	VE_PARSER_ENUM_ITEM("uav", D3D12_ROOT_PARAMETER_TYPE_UAV);
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
