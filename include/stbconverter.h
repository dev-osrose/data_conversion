#ifndef __STBCONVERTER_H__
#define __STBCONVERTER_H__

namespace ROSE {
  class StbConverter {
  private:
  enum ItemType {
    FACE = 1,
    CAP,
    BODY,
    ARMS,
    FOOT,
    BACK,
    JEWEL,
    WEAPON,
    SUB_WEAPON,
    USEITEM,
    JEMITEM,
    NATURAL,
    QUESTITEM,
    PAT,
    MAX_ITEM_TYPES
  };

	public:
		StbConverter();
		~StbConverter();
		
		void Start();
  };
};

#endif