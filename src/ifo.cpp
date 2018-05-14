#include <memory>

#include "ifo.h"
#include "filesystem.hpp"
#include "safedelete.hpp"
#include "scopedpointer.hpp"


namespace ROSE {
	IFO::IFO() : mDataOffsets(0), mIfoData(0), mColCount(0), mRowCount(0) {}
	IFO::IFO(const char* path) : mDataOffsets(0), mIfoData(0), mColCount(0), mRowCount(0) {
		Load(path);
	}

	IFO::~IFO(){
		SAFE_DELETE_ARRAY(mIfoData);
		SAFE_DELETE_ARRAY(mDataOffsets);
	}

	bool IFO::Load(const char* path){
		ScopedPointer<File> fh(FILE_SYS()->OpenFile(path, "rb"));
		if(!fh) return false;

//DWORD block_count
//:FOREACH( block_count )
//    DWORD block_type
//    DWORD block_offset
//:ENDFOR		
		uint32_t block_count = 0;
		fh->Read(block_count);
		
		std::vector<BlockInfo> blockList;
		for(uint32_t index = 0; index < block_count; ++index) {
			BlockInfo nblock;
			fh->Read(nblock.block_type);
			fh->Read(nblock.block_offset);
			blockList.push_back(nblock);
		}
		
		for(auto block : blockList)
		{
			switch(static_cast<BlockTypes>(block.block_type))
			{
				case BlockTypes::Economy:
					// This one is debatiable. Not sure if we are going to need this
					break;
				case BlockTypes::DecoObjects:
					break;
				case BlockTypes::NpcLocations:
					break;
				case BlockTypes::BuildingObjects:
					break;
				case BlockTypes::AnimatableObjects:
					break;
				case BlockTypes::MonsterSpawners:
					break;
				case BlockTypes::WarpGates:
					break;
				case BlockTypes::Triggers:
					break;
				default:
					// We don't care about any not listed above
					break;
			}
		}

//:FOREACH( block_count )
//    :SEEK( block_offset )
//    :IF( block_type == 0 ) //Economy Data
//        BYTE[0x56] unknown
//    :ENDIF
//    :IF( block_type == 7 ) //Unknown
//        DWORD x_count
//        DWORD y_count
//        :FOREACH( x_count )
//            :FOREACH( y_count )
//                BYTE[17] unknown
//            :ENDFOR
//        :ENDFOR
//        :GOTO NEXT BLOCK!
//    :ENDIF
//    :IF( block_type == 9 ) //Water Planes
//        FLOAT basic_float
//        DWORD entry_count
//        :FOREACH( entry_count )
//            FLOAT x1
//            FLOAT y1
//            FLOAT z1
//            FLOAT x2
//            FLOAT y2
//            FLOAT z2
//        :ENDFOR
//        :GOTO NEXT BLOCK!
//    :ENDIF
//    //All blocks except 0, 7 and 9 have base format:
//    DWORD entry_count
//    :FOREACH( entry_count )
//        BSTR str_data
//        WORD unknown1
//        WORD event_id
//        DWORD obj_type
//        DWORD obj_id
//        DWORD map_pos_x
//        DWORD map_pos_y
//        FLOAT quaternion_x
//        FLOAT quaternion_y
//        FLOAT quaternion_z
//        FLOAT quaternion_w
//        FLOAT position_x
//        FLOAT position_y
//        FLOAT position_z
//        FLOAT scale_x
//        FLOAT scale_y
//        FLOAT scale_z
//        :IF( block_type == 1 ) //Objects ( LIST_TERRAIN_OBJECT_*.STB, LIST_DECO_*.ZSC )
//            //No Extra Data
//        :ENDIF
//        :IF( block_type == 2 ) //NPC Locations
//            DWORD unknown1
//            BSTR con_file
//        :ENDIF
//        :IF( block_type == 3 ) //Objects 2 ( Houses ) ( LIST_CNST_*.STB, LIST_CNST_*.ZSC )
//            //No Extra Data
//        :ENDIF
//        :IF( block_type == 4 ) //Sound Effects
//            BSTR path
//            DWORD unknown1
//            DWORD unknown2
//        :ENDIF
//        :IF( block_type == 5 ) //Effects
//            BSTR path
//        :ENDIF
//        :IF( block_type == 6 ) //Objects 3 ( Animatable ) ( LIST_MORPH.STB )
//            //No Extra Data
//        :ENDIF
//        :IF( block_type == 8 ) //Monster Spawn Points
//            BSTR spawn_name
//            DWORD basic_mobs
//            :FOREACH( basic_mobs )
//                BSTR mob_name
//                DWORD monster_id
//                DWORD amount
//            :ENDFOR
//            DWORD tactic_mobs
//            :FOREACH( tactic_mobs )
//                BSTR mob_name
//                DWORD monster_id
//                DWORD amount
//            :ENDFOR
//            DWORD interval
//            DWORD limit_count
//            DWORD range
//            DWORD tactic_points
//        :ENDIF
//        :IF( block_type == 10 ) //Warp Gates
//            //No Extra Data
//        :ENDIF
//        :IF( block_type == 11 ) //Unknown
//            //No Extra Data
//        :ENDIF
//        :IF( block_type == 12 ) //Triggers
//            BSTR qsd_trigger
//            BSTR lua_trigger
//        :ENDIF
//    :ENDFOR
//:ENDFOR

		fh->Close();
		return true;
	}
};