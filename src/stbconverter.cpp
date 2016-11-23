#include "file.hpp"
#include "stb.h"
#include "stl.h"
#include "stbconverter.h"


namespace ROSE {
  StbConverter::StbConverter() {}
  StbConverter::~StbConverter() {}
  
  void StbConverter::Start() {
    ROSE::STB* itemTbl = nullptr;
    ROSE::STL* itemStrTbl = nullptr;
    std::string stbName[ItemType::MAX_ITEM_TYPES] = {
      "",
      "list_faceitem.stb",
      "list_cap.stb",
      "list_body.stb",
      "list_arms.stb",
      "list_foot.stb",
      "list_back.stb",
      "list_jewel.stb",
      "list_weapon.stb",
    }; 
    
    std::string stlName[ItemType::MAX_ITEM_TYPES] = {
      "",
      "list_faceitem_s.stl",
      "list_cap_s.stl",
      "list_body_s.stl",
      "list_arms_s.stl",
      "list_foot_s.stl",
      "list_back_s.stl",
      "list_jewel_s.stl",
      "list_weapon_s.stl",
    };
    
    for(int type = 0; type < ItemType::MAX_ITEM_TYPES; ++type) {
      itemTbl = new ROSE::STB();
      itemStrTbl = new ROSE::STL();
      if(stbName[type].empty() || stlName[type].empty()) continue;
      
      LOG("Loading {} w/ {}", stbName[type], stlName[type]);
      if(itemTbl->Load(stbName[type].c_str()) == false) {
        SAFE_DELETE(itemTbl);
        continue;
      }
      if(itemStrTbl->Load(stlName[type].c_str()) == false) {
        SAFE_DELETE(itemStrTbl);
        continue;
      }
        
      for(int i = 1; i < itemTbl->Rows(); ++i) {
        int id = itemStrTbl->GetIDByStr( itemTbl->GetString(i, itemTbl->Columns()-1) );
        if(id != -1) {
          
          unsigned int subtype, price_buy, weight, attack, defense, range, slots, equip_jobs, equip_genders, equip_level, refinable, view_model;
          subtype = price_buy = weight = attack = defense = range = slots = equip_jobs = equip_genders = equip_level = refinable = view_model = 0;
          float price_sell = 0.0f;
          
          
          // use restriction == col 3
          //#define ITEM_NO_RESTRICTION				0x00
          //#define ITEM_DONT_SELL					0x01
          //#define ITEM_DONT_DROP_EXCHANGE			0x02
          //#define ITEM_DONT_SELL_DROP_EXCHANGE	0x03
          //#define	ITEM_ENABLE_KEEPING				0x04
          
          subtype = itemTbl->GetInt(i, 4);
          price_buy = itemTbl->GetInt(i, 5);
          price_sell = itemTbl->GetFloat(i, 6); // This might actually be a float or double
          weight = itemTbl->GetInt(i, 7);
          //quality
          // icon ID
          view_model = itemTbl->GetInt(i, 10);
          equip_jobs = itemTbl->GetInt(i, 16);
          // col 17 and 18 == required union id for equipping
          defense = itemTbl->GetInt(i, 31);
          
          switch(type) {
            case ItemType::BACK:
            case ItemType::FOOT: {
              //move speed == col 33
              break;
            }
            
            case ItemType::WEAPON: {
              slots = itemTbl->GetInt(i, 30);
              range = itemTbl->GetInt(i, 33);
              attack = itemTbl->GetInt(i, 35);
              break;
            }
            case ItemType::SUB_WEAPON: {
              slots = itemTbl->GetInt(i, 30);
              break;
            }
            default:
              break;
          }
          //#define ITEM_NEED_DATA_CNT				2
          //#define	ITEM_NEED_DATA_TYPE(T,I,C)		g_pTblSTBs[T]->m_ppDATA[ I ][ 19+(C*2) ]
          //#define	ITEM_NEED_DATA_VALUE(T,I,C)		g_pTblSTBs[T]->m_ppDATA[ I ][ 20+(C*2) ]
          //equip_level and equip_genders will be in the data cols listed above
          
          //fmt::MemoryWriter out;
          //for (int i = 0; i < itemTbl->Columns(); i++) {
          //  // Dump all of the data in each col into a comment in the script field, that way we can do manually update the scripts and stuff
          //  //out.write( "0x{0:02x} ", raw_ptr[i] );
          //}
            
          LOG("INSERT into item_db(id, name, desc, type, subtype, price_buy, price_sell, weight, attack, defense, range, slots, equip_jobs, equip_genders, equip_level, refinable, view_id) \
            values({0}, \"{1}\", \"{2}\", {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}, {11}, {12}, {13}, {14}, {15}, {16});",
            i,
            itemStrTbl->GetText( id ),
            itemStrTbl->GetComment( id ),
            type,
            subtype,
            price_buy,
            price_sell,
            weight,
            attack,
            defense,
            range,
            slots,
            equip_jobs,
            equip_genders,
            equip_level,
            refinable, 
            view_model//view_id == model ID on client side
            //script == lua script with functions of OnCreate() OnRemove() OnEquipped() OnUnequip() OnDrop() OnPickup() OnUse()
            );
        }
      }
      
      SAFE_DELETE(itemStrTbl);
      SAFE_DELETE(itemTbl);
    }
  }
};