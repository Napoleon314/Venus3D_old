//////////////////////////////////////////////////////////////////////////////
//
//  Copyright 2014 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//
//////////////////////////////////////////////////////////////////////////////
#pragma once

#include "matrix3.h"
#include "AssetManagement/AssetUser.h"

//==================================================================================================
// class ParamBlockDesc2
//==================================================================================================

inline const ParamDef* ParamBlockDesc2::GetParamDefByIndex(const unsigned int parameterIndex) const
{
    if(DbgVerify((paramdefs != nullptr) && (parameterIndex < count)))
    {
        return &(paramdefs[parameterIndex]);
    }
    else
    {
        return nullptr;
    }
}

//==================================================================================================
// class IParamBlock2
//==================================================================================================

inline Color IParamBlock2::GetColor(const ParamID id, const TimeValue t, Interval& validity, const int tabIndex)
{
    Color val(0.0f, 0.0f, 0.0f);
    DbgVerify(GetValue(id, t, val, validity, tabIndex));
    return val;
}

inline AColor IParamBlock2::GetAColor(const ParamID id, const TimeValue t, Interval& validity, const int tabIndex)
{
    AColor val(0.0f, 0.0f, 0.0f, 0.0f);
    DbgVerify(GetValue(id, t, val, validity, tabIndex));
    return val;
}

inline Point3 IParamBlock2::GetPoint3(const ParamID id, const TimeValue t, Interval& validity, const int tabIndex)
{
    Point3 val(0.0f, 0.0f, 0.0f);
    DbgVerify(GetValue(id, t, val, validity, tabIndex));
    return val;
}

inline Point4 IParamBlock2::GetPoint4(const ParamID id, const TimeValue t, Interval& validity, const int tabIndex)
{
    Point4 val(0.0f, 0.0f, 0.0f, 0.0f);
    DbgVerify(GetValue(id, t, val, validity, tabIndex));
    return val;
}

inline int IParamBlock2::GetInt(const ParamID id, const TimeValue t, Interval& validity, const int tabIndex)
{
    int val = 0;
    DbgVerify(GetValue(id, t, val, validity, tabIndex));
    return val;
}

inline float IParamBlock2::GetFloat(const ParamID id, const TimeValue t, Interval& validity, const int tabIndex)
{
    float val = 0.0f;
    DbgVerify(GetValue(id, t, val, validity, tabIndex));
    return val;

}

inline TimeValue IParamBlock2::GetTimeValue(const ParamID id, const TimeValue t, Interval& validity, const int tabIndex)
{
    TimeValue val = 0;
    DbgVerify(GetValue(id, t, val, validity, tabIndex));
    return val;
}



inline const MCHAR* IParamBlock2::GetStr(const ParamID id, const TimeValue t, Interval& validity, const int tabIndex)
{
    const MCHAR* val = nullptr;
    DbgVerify(GetValue(id, t, val, validity, tabIndex));
    return val;
}

inline Mtl* IParamBlock2::GetMtl(const ParamID id, const TimeValue t, Interval& validity, const int tabIndex)
{
    Mtl* val = nullptr;
    DbgVerify(GetValue(id, t, val, validity, tabIndex));
    return val;
}


inline Texmap* IParamBlock2::GetTexmap(const ParamID id, const TimeValue t, Interval& validity, const int tabIndex)
{
    Texmap* val = nullptr;
    DbgVerify(GetValue(id, t, val, validity, tabIndex));
    return val;
}

inline PBBitmap* IParamBlock2::GetBitmap(const ParamID id, const TimeValue t, Interval& validity, const int tabIndex)
{
    PBBitmap* val = nullptr;
    DbgVerify(GetValue(id, t, val, validity, tabIndex));
    return val;
}

inline INode* IParamBlock2::GetINode(const ParamID id, const TimeValue t, Interval& validity, const int tabIndex)
{
    INode* val = nullptr;
    DbgVerify(GetValue(id, t, val, validity, tabIndex));
    return val;
}

inline ReferenceTarget* IParamBlock2::GetReferenceTarget(const ParamID id, const TimeValue t, Interval& validity, const int tabIndex)
{
    ReferenceTarget* val = nullptr;
    DbgVerify(GetValue(id, t, val, validity, tabIndex));
    return val;
}

inline IParamBlock2* IParamBlock2::GetParamBlock2(const ParamID id, const TimeValue t, Interval& validity, const int tabIndex)
{
    IParamBlock2* val = nullptr;
    DbgVerify(GetValue(id, t, val, validity, tabIndex));
    return val;
}

inline Matrix3 IParamBlock2::GetMatrix3(const ParamID id, const TimeValue t, Interval& validity, const int tabIndex)
{
    Matrix3 val(TRUE);
    DbgVerify(GetValue(id, t, val, validity, tabIndex));
    return val;
}

inline MaxSDK::AssetManagement::AssetUser IParamBlock2::GetAssetUser(const ParamID id, const TimeValue t, Interval& validity, const int tabIndex)
{
    MaxSDK::AssetManagement::AssetUser val;
    DbgVerify(GetValue(id, t, val, validity, tabIndex));
    return val;
}

template<typename T>
inline bool IParamBlock2::SetValueByName(const MCHAR* const paramName, const T& value, const TimeValue t, const int tabIndex)
{
    // Find the parameter that matches the given name
    ParamBlockDesc2* const pbdesc = GetDesc();
    if(DbgVerify(pbdesc != nullptr))
    {
        const int param_index = pbdesc->NameToIndex(paramName);
        if(DbgVerify(param_index >= 0))
        {
            const ParamDef* const param_def = pbdesc->GetParamDefByIndex(param_index);
            if(DbgVerify(param_def != nullptr))
            {
                return DbgVerify(SetValue(param_def->ID, t, value, tabIndex) != 0);
            }
        }
    }

    return false;
}

template<typename T>
inline bool IParamBlock2::GetValueByName(const MCHAR* const paramName, const TimeValue t, T& value, Interval& validity, const int tabIndex)
{
    // Find the parameter that matches the given name
    ParamBlockDesc2* const pbdesc = GetDesc();
    if(DbgVerify(pbdesc != nullptr))
    {
        const int param_index = pbdesc->NameToIndex(paramName);
        if(DbgVerify(param_index >= 0))
        {
            const ParamDef* const param_def = pbdesc->GetParamDefByIndex(param_index);
            if(DbgVerify(param_def != nullptr))
            {
                return DbgVerify(GetValue(param_def->ID, t, value, validity, tabIndex) != 0);
            }
        }
    }

    return false;
}

