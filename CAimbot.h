#ifndef __CAIMBOT_H__
#define __CAIMBOT_H__

#include "sdk.h"

class cCAimbot
{
public:
	cCAimbot();
	//----------------------------------------------
	int		iGetTarget( void );
	void	DropTarget( void );
	void	AimAtTarget ( CUserCmd* pCommand );
	bool	GetVisible( Vector& vecAbsStart, Vector& vecAbsEnd, C_BaseEntity* pBaseEnt );
	void	MakeVector( QAngle angle, QAngle& vector );
	void	CalcAngle( Vector &src, Vector &dst, QAngle &angles );
	float	GetFov( QAngle angle, Vector src, Vector dst );
	int		m_nTarget;
	float	flBestDist;
	Vector	vPlayer;							//see my name
	Vector	PredictedTargetPosition;			//see my name
};

#endif
