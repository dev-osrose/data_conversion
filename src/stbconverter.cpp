#include "file.hpp"
#include "stb.h"
#include "stl.h"
#include "stbconverter.h"
#include "flatfilesystem.hpp"
#include "safedelete.hpp"
#include "scopedpointer.hpp"


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
      "list_subwpn.stb",
      "list_useitem.stb",
      "list_jemitem.stb",
      "list_natural.stb",
      "list_questitem.stb",
      "list_pat.stb",
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
      "list_subwpn_s.stl",
      "list_useitem_s.stl",
      "list_jemitem_s.stl",
      "list_natural_s.stl",
      "list_questitem_s.stl",
      "list_pat_s.stl",
    };

    fmt::MemoryWriter outSql;
    
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

          std::string name = itemStrTbl->GetText(id);
          std::string desc = itemStrTbl->GetComment(id);

          if (name.empty()) continue;
          if (desc.empty()) continue;
          
          double price_sell = 0.0f;
          unsigned int subtype, price_buy, weight, attack, defense, range, slots, equip_jobs, equip_genders, equip_level, refinable, view_model;
          subtype = price_buy = weight = attack = defense = range = slots = equip_jobs = equip_genders = equip_level = refinable = view_model = 0;
          unsigned int attack_speed, magic, move_speed, usage_restrictions;
          attack_speed = magic = move_speed = usage_restrictions = 0;
          std::string script = "";
          
          usage_restrictions = itemTbl->GetInt(i, 3);
          subtype = itemTbl->GetInt(i, 4);
          price_buy = itemTbl->GetInt(i, 5);
          price_sell = itemTbl->GetDouble(i, 6); // This might actually be a float or double
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
              move_speed = itemTbl->GetInt(i, 33);
              break;
            }
            
            case ItemType::WEAPON: {
              slots = itemTbl->GetInt(i, 30);
              range = itemTbl->GetInt(i, 33);
              attack = itemTbl->GetInt(i, 35);
              attack_speed = itemTbl->GetInt(i, 36);
              magic = itemTbl->GetInt(i, 37);
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
          fmt::MemoryWriter out;
#define OUTPUT_STB_DATA_COMMENT
#ifdef OUTPUT_STB_DATA_COMMENT
          out.write("--[[ ");
          for (int col = 0; col < itemTbl->Columns(); col++) {
            // Dump all of the data in each col into a comment in the script field, that way we can do manually update the scripts and stuff
            out.write( "COLUMN {} == \"{}\" \n", col, itemTbl->GetString(i, col) );
          }
          out.write(" --]]\n\n");
#endif

          //#define ITEM_NO_RESTRICTION				0x00
          //#define ITEM_DONT_SELL					0x01
          //#define ITEM_DONT_DROP_EXCHANGE			0x02
          //#define ITEM_DONT_SELL_DROP_EXCHANGE	0x03
          //#define	ITEM_ENABLE_KEEPING				0x04
          out.write("use_restriction = {}\n", usage_restrictions);
          // write out the requirement stat table
          out.write("reqTable = {}\n", "{}");
          for (int reqCount = 0; reqCount < 2; ++reqCount) {
            std::string type = itemTbl->GetString(i, 19 + (reqCount * 2));
            std::string value = itemTbl->GetString(i, 20 + (reqCount * 2));
            if (type.empty()) continue;
            
            out.write("reqTable[{}] = {}\n", reqCount + 1, "{}");
            out.write("reqTable[{}].type = {}\n", reqCount + 1, type);
            out.write("reqTable[{}].value = {}\n", reqCount + 1, value);
          }
          out.write("\n\n");

          // write out the bonus stat table
          out.write("bonusTable = {}\n", "{}");
          for (int bonusCount = 0; bonusCount < 2; ++bonusCount) {
            std::string type = itemTbl->GetString(i, 24 + (bonusCount * 3));
            std::string value = itemTbl->GetString(i, 25 + (bonusCount * 3));
            if (type.empty()) continue;

            out.write("bonusTable[{}] = {}\n", bonusCount + 1, "{}");
            out.write("bonusTable[{}].type = {}\n", bonusCount + 1, type);
            out.write("bonusTable[{}].value = {}\n", bonusCount + 1, value);
          }
          out.write("\n\n");

          //TODO Update these lua functions to have default behavor
          out.write("\n\
function OnInit()\n\
end\n\
\n\
function OnCreate()\n\
end\n\
\n\
function OnRemove()\n\
end\n\
\n\
function OnEquip()\n\
  for i, data in ipairs(reqTable) do\n\
    if data.value > GetCurrentAttr(data.type) then\n\
      return false\n\
    end\n\
  end\n\
\n\
  for i, data in ipairs(bonusTable) do\n\
    AddBonusAttr(data.type, data.value)\n\
  end\n\
  return true\n\
end\n\
\n\
function OnUnequip()\n\
  for i, data in ipairs(bonusTable) do\n\
    RemoveBonusAttr(data.type, data.value)\n\
  end\n\
end\n\
\n\
function OnDrop()\n\
end\n\
\n\
function OnPickup()\n\
end\n\
\n\
function OnUse()\n\
end");

          script = out.c_str();

          auto replaceString = [](std::string& str, std::string token = "\'", std::string replacew = "\\\'") {
            auto pos = str.find(token.c_str(), 0);
            while (pos != std::string::npos) {
              str.replace(pos, 1, fmt::format(replacew.c_str(), token.c_str()).c_str());
              pos = str.find(token.c_str(), pos + 2);
            }
          };

          auto removeString = [](std::string& str, std::string token = "\'") {
            auto pos = str.find(token.c_str(), 0);
            while (pos != std::string::npos) {
              str.replace(pos, 1, "");
              pos = str.find(token.c_str(), pos + 2);
            }
          };
          
          replaceString(name, "\"");
          replaceString(desc, "\"");
          replaceString(script, "\'");

          // TODO replace unicode characters
          replaceString(desc, "…", "."); // This doesn't seem to work D: (Conjurer's Wand in the weapon stb has this character in it)

          outSql.write("INSERT into item_db(id, name, `desc`, type, subtype, price_buy, price_sell, weight, attack, defense, `range`, slots, equip_jobs, equip_genders, equip_level, refinable, view_id, script) \
            values({0}, \"{1}\", \"{2}\", {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}, {11}, {12}, {13}, {14}, {15}, {16}, '{17}');\n",
            i,
            name,
            desc,
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
            view_model,//view_id == model ID on client side
            script //== lua script with functions of OnCreate() OnRemove() OnEquipped() OnUnequip() OnDrop() OnPickup() OnUse()
            );
        }
      }

      ScopedPointer<File> fh(FlatFileSystem("./").OpenFile(fmt::format("{}.sql", stbName[type]).c_str(), "w"));
      if (!fh) continue;
      fh->WriteTerminatedString(outSql.str());
      fh->Close();
      
      outSql.clear();

      SAFE_DELETE(itemStrTbl);
      SAFE_DELETE(itemTbl);
    }
  }
};