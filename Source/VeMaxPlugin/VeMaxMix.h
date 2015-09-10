////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMaxPlugin
//  File name:   VeMaxMix.h
//  Created:     2015/09/10 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

#include <3dsmaxport.h>
#include <stdmat.h>

static Class_ID mixClassID(MIX_CLASS_ID, 0);

#define NMIXSUBTEX 3    // number of texture map slots
#define NCOLS 2      // number of color swatches
#define MIX_CLASSNAME _T("")//GetString(IDS_RB_FOG)

//--------------------------------------------------------------
// Mix: A Mix texture map
//--------------------------------------------------------------
#define PB_REF 0
#define SUB1_REF 1
#define SUB2_REF 2
#define SUB3_REF 3
#define PB_TEXOUT 4

class MixDlgProc;

class Mix : public MultiTex {
public:
	enum
	{
		mix_mix, mix_curvea, mix_curveb, mix_usecurve,
		mix_color1, mix_color2,
		mix_map1, mix_map2, mix_mask,
		mix_map1_on, mix_map2_on, mix_mask_on, // main grad params 
		mix_output
	};

	static ParamDlg* texoutDlg;
	static MixDlgProc *paramDlg;
	TexHandle *texHandle[3];
	int useSubForTex[3];
	int numTexHandlesUsed;
	int texOpsType[3];
	Interval texHandleValid;
	Color col[NCOLS];
	IParamBlock2 *pblock;   // ref #0
	Texmap* subTex[NMIXSUBTEX];  // 3 More refs
	BOOL mapOn[NMIXSUBTEX];
	TextureOutput *texout; // ref #4
	Interval ivalid;
	float mix;
	BOOL useCurve;
	float crvA;
	float crvB;
	BOOL rollScroll;
public:
	BOOL Param1;
	Mix() = default;
	~Mix() {
		DiscardTexHandles();
	}
	//ParamDlg* CreateParamDlg(HWND hwMtlEdit, IMtlParams *imp);
	//void Update(TimeValue t, Interval& valid);
	//void Init();
	//void Reset();
	Interval Validity(TimeValue t) { Interval v; Update(t, v); return ivalid; }

	void SetOutputLevel(TimeValue t, float v) { texout->SetOutputLevel(t, v); }
	//void SetColor(int i, Color c, TimeValue t);
	//void SetMix(float f, TimeValue t);
	//void SetCrvA(float f, TimeValue t);
	//void SetCrvB(float f, TimeValue t);
	//void NotifyChanged();
	//void SwapInputs();
	//void EnableStuff();

	//float mixCurve(float x);

	//// Evaluate the color of map for the context.
	//AColor EvalColor(ShadeContext& sc);
	//float EvalMono(ShadeContext& sc);
	//AColor EvalFunction(ShadeContext& sc, float u, float v, float du, float dv);

	// For Bump mapping, need a perturbation to apply to a normal.
	// Leave it up to the Texmap to determine how to do this.
	//Point3 EvalNormalPerturb(ShadeContext& sc);

	// Methods to access texture maps of material
	int NumSubTexmaps() { return NMIXSUBTEX; }
	Texmap* GetSubTexmap(int i) { return subTex[i]; }
	//void SetSubTexmap(int i, Texmap *m);
	//TSTR GetSubTexmapSlotName(int i);

	Class_ID ClassID() { return mixClassID; }
	SClass_ID SuperClassID() { return TEXMAP_CLASS_ID; }
	void GetClassName(TSTR& s) { s = MIX_CLASSNAME; }
	void DeleteThis() { delete this; }

	int NumSubs() { return 2 + NMIXSUBTEX; }
	//Animatable* SubAnim(int i);
	//TSTR SubAnimName(int i);
	int SubNumToRefNum(int subNum) { return subNum; }

	// From ref
	int NumRefs() { return 2 + NMIXSUBTEX; }
	//RefTargetHandle GetReference(int i);
	//void SetReference(int i, RefTargetHandle rtarg);

	//RefTargetHandle Clone(RemapDir &remap = DefaultRemapDir());
	//RefResult NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget,
	//	PartID& partID, RefMessage message);

	// IO
	//    BOOL loadOnChecks;
	//IOResult Save(ISave *isave);
	//IOResult Load(ILoad *iload);

	// JBW: direct ParamBlock access is added
	int   NumParamBlocks() { return 1; }               // return number of ParamBlocks in this instance
	IParamBlock2* GetParamBlock(int i) { return pblock; } // return i'th ParamBlock
	IParamBlock2* GetParamBlockByID(BlockID id) { return (pblock->ID() == id) ? pblock : NULL; } // return id'd ParamBlock
	//BOOL SetDlgThing(ParamDlg* dlg);
	void DiscardTexHandles() {
		for (int i = 0; i < 3; i++) {
			if (texHandle[i]) {
				texHandle[i]->DeleteThis();
				texHandle[i] = NULL;
			}
		}
		texHandleValid.SetEmpty();
	}

	// Multiple map in vp support -- DS 4/24/00
	BOOL SupportTexDisplay() { return TRUE; }
	//void ActivateTexDisplay(BOOL onoff);
	BOOL SupportsMultiMapsInViewport() { return TRUE; }
	//void SetupGfxMultiMaps(TimeValue t, Material *mtl, MtlMakerCallback &cb);

	// Multiple map code
	/*void StuffAlpha(BITMAPINFO* bi1, BITMAPINFO *bi2, BOOL invert);
	void StuffConstAlpha(float falpha, BITMAPINFO *bmi);
	void ConstAlphaBlend(BITMAPINFO *bi1, BITMAPINFO *bi2, float a);
	void ConstScale(BITMAPINFO *bi, float s);
	void AlphaBlendMaps(BITMAPINFO *bi1, BITMAPINFO *bi2, BITMAPINFO *balph);
	void FixupAlpha(BITMAPINFO *bi);
	void SetTexOps(Material *mtl, int i, int type);
	void SetHWTexOps(IHardwareMaterial *pIHWMat, int ntx, int type);*/

	// From Texmap
	bool IsLocalOutputMeaningful(ShadeContext& sc) { return true; }
};
