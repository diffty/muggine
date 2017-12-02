#ifndef TS_WORKGUY_THING_HPP
#define TS_WORKGUY_THING_HPP

#include "ts_draggable_thing.hpp"



class WorkguyThing : public DraggableThing
{
private:


public:
	WorkguyThing(SpriteSheet* pSprSht, uint uFrameNb, vect2df_t vPos);
	~WorkguyThing();

	void onUsing();
	void onEndUsing(MainCharacter* pChar);
};

typedef struct work_job_t {
	WorkguyThing* m_pEmployer;
	float m_iPrice;
	double m_fTimeStarted;
} work_job_t;

#endif
