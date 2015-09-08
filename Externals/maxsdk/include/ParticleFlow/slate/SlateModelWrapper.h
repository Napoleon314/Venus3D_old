#pragma once

#include "../../maxheap.h"
#include "../../Ref.h"
#include "../PFExport.h"

// the original wrapped SlateSDK class
namespace Slate
{
	class IModel;
};

namespace MaxSDK
{
	namespace ParticleFlow
	{		
		/**
		* SlateModelWrapper is a simple wrapper class that holds an internal pointer to a particle flow slate model.
		* Public particle flow actions/operators need to derive from this class when they want to show up in the new slate particle view.
		* The memory management of the internal model is done by the wrapper.
		* This wrapper class has been introduced temporarily in order to keep the MaxSDK clean from any appearances of the currently non public SlateSDK.
		* When the SlateSDK is final this wrapper class can be replaced again, with it's according slate model class.
		*/
		class PFExport SlateModelWrapper : public MaxHeapOperators
		{
		public:

			SlateModelWrapper();
			SlateModelWrapper( const SlateModelWrapper& slateModelWrapper ); // creates a new internal slate model
			virtual ~SlateModelWrapper();

			/** Returns a pointer to the original wrapped slate model.*/
			Slate::IModel*	GetModel() { return mModel; }


			// connection related stuff -------------------------------------------
			enum Direction
			{
				Direction_None = 0x0, // no connections at all
				Direction_In = 0x1, // accepts uni- and bi-directional connections
				Direction_Out = 0x2, // accepts only uni-directional connections : other OUT -> this IN
				Direction_Both = 0x3, // accepts only uni-directional connections : this OUT -> other IN
			};

			/** Sets the connection direction of the wrapped slate model.
			* In case of a test operator it is Direction_Out for simple action operators 
			* its just Direction_None. 
			* @param [in] direction The connection of the wrapped model.
			*/
			void SetDirection( Direction direction );

			/** Helpers for the public maxsdk sample class PFOperatorDisplay.
			* Notify slate model listeners like the ui controller about the color change, 
			* so that it can update the color of the slate color swatch.
			*/
			void NotifyListenersDisplayColorChanged();

			/** Helpers for the public maxsdk sample class PFOperatorDisplay.
			* Defines the display operator should use a slate color swatch for its UI edit control.
			*/
			void DefineColorSwatchUI();

			/** Helpers for the public maxsdk sample class PFOperatorDisplay.
			* Updates visibility state of Color Swatch UI
			*/
			void RefreshColorSwatchVisibility();

			/** Need to be called by NotifyTarget( int message, ReferenceMaker* hMaker ) in the object that derives from SlateModelWrapper.
			* It is internally used by slate to detect if the according INode for a particle flow action gets deleted.
			*/
			void RefMakerNotifyTarget( int message, ReferenceMaker* hMaker );

		private:	
			/** Need to be called in the constructors
			*/
			void init(void);

			// The wrapped slate model. Allocation and deallocation is done by this wrapper.
			Slate::IModel* mModel; 

		};
	};
};

