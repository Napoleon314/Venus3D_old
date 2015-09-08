/**********************************************************************
 *<
	FILE: IMergeable.h

	DESCRIPTION: This class allows references that are not attached to INodes to be mergeable.  You
		just need to fill out SetMergeableRef and override PostMerge 

	CREATED BY: Peter Watje

	HISTORY: Created 5/14/2012

 *>	Copyright (c) 2012, All Rights Reserved.
 **********************************************************************/

#pragma once

#include "ref.h"
#include "inode.h"
#include "notify.h"


//this is the interface ID for the Mergeable interface
#define MERGEABLE_INTERFACE 0x00010c9b

/*! 
Description:  This class allows referenceTargets that are not INodes to be mergeable through the File->Merge command.  
The referenceTarget that you want to be mergeable must derive from Mergeable.  You just then need to override GetMergeableRef, 
GetMergeableRefName, and PostMerge.  

Code Example to make the Brightness/Contrast effect mergeable.

You would derive your class from the Mergeable

\code 
class BriteConEffect: public Effect, public Mergeable {
\endcode 

Then make sure you return the Mergeable Interface from GetInterface when it is asked 

\code 
void* BriteConEffect::GetInterface(ULONG id)
{
	switch(id)
	{
		case MERGEABLE_INTERFACE : return (Mergeable *) this;
			break;
		default: return Effect::GetInterface(id);
			break;
	}
}
\endcode 

Next just need to implement GetMergeableRef and GetMergeableRefName which return the data needed to do the merge

\code 
ReferenceTarget* BriteConEffect::GetMergeableRef()
{
	return this;
}
TSTR BriteConEffect::GetMergeableRefName()
{
	return GetName();
}
\endcode 

Finally you just need to implement PostMerge where you do your work to hook back up  
your reference after the merge

\code 
void BriteConEffect::PostMerge()
{
	GetCOREInterface()->AddEffect(this);
}
\endcode 

*/
class Mergeable : public MaxHeapOperators
{
public:
	CoreExport Mergeable();
	CoreExport virtual ~Mergeable();

	/*! This needs to be implemented to return the ReferenceTarget that will be mergeable.  This is the ReferenceTarget that is 
	derived from Mergeable.  If it does not derive from Mergeable, PostMerge will not be called on it after merging.
	*/
	CoreExport virtual ReferenceTarget* GetMergeableRef() = 0;


	/*! This returns the name that you want to appear in the merge list box 
	*/
	CoreExport virtual TSTR GetMergeableRefName() = 0;

	/*! This is called after the reference has been merged.  At this point it needs to be attached to something
	otherwise it will be lost.  For instance if this was an effect it would have to be attached to scene graphs effects list.
	*/
	CoreExport virtual void PostMerge() = 0;

	/*! This is called before saving.  If you are in a state where you don't need to be saved, then return false.  
	Otherwise by default this returns true.
	*/
	CoreExport virtual bool SaveForMerge() {return true;}
	
	/*! The name returned from GetMergeableRefName() will be made unique in the merge list box, unless this is implemented to return false.
	*/
	CoreExport virtual bool MakeMergeNameUnique() {return true;}
	

protected:



private:

	static void Notify(void *param, NotifyInfo *info);

	void PreSaveInternal();
	void PostSaveInternal();
	void PreMergeInternal();
	void PostMergeInternal();

	bool mSkipMerge;				//this is a bool used to track which refTargs are already in the scene and which ones are to be
									//merged so we don't accidently duplicate something that was already there.
	INode *mTempNode;				//pointer to the temporary node we create so it will show up in the merge list

};




