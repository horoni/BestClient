#ifndef BALANCEBOT_H
#define BALANCEBOT_H

#include <game/client/component.h>

class CSHBalanceBot : public CComponent
{
public:
	int Sizeof() const override { return sizeof(*this); }

	struct CTargetInfo;

	void Balance();
	bool IsValidVerticalPosition(const vec2& TargetPos, const vec2& LocalPos) const;
	std::optional<CTargetInfo> FindTarget(const vec2& LocalPos, float MaxDistance) const;

	struct CTargetInfo {
		int m_Id;
		vec2 m_Pos;
		float m_Distance;

		CTargetInfo(int Id, const vec2& Pos, float Distance) :
			m_Id(Id), m_Pos(Pos), m_Distance(Distance) {}
	};
};
#endif
