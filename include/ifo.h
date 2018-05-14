#ifndef STB_H
#define STB_H

namespace ROSE {
	class IFO {
	public:
		IFO();
		IFO(const char* path);
		~IFO();
		
		enum BlockTypes : uint32_t { Economy, DecoObjects, NpcLocations, BuildingObjects, SoundEffects, Effects, AnimatableObjects, MonsterSpawners, WarpGates, Unknown, Triggers };
		struct BlockInfo {
			uint32_t block_type;
			uint32_t block_offset;
			BlockInfo() : block_type(0), block_offset(0) {}
		};

		bool Load(const char* path);

	private:
		unsigned int mRowCount;
		unsigned int mColCount;

		long mBytes;
		long* mDataOffsets;
		char* mIfoData;
	};
};

#endif