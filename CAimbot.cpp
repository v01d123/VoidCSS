#include "CAimbot.h"
#include "sdk.h"

cCAimbot::cCAimbot( void )
{
	flBestDist = 99999.9f;
	m_nTarget = -1;
}

void cCAimbot::DropTarget( void )
{
	flBestDist = 99999.9f;
	m_nTarget = -1;
}

int	cCAimbot::iGetTarget( void )
{
	return m_nTarget;
}

void cCAimbot::MakeVector( QAngle angle, QAngle& vector ) 
{ 
	float pitch; 
	float yaw; 
	float tmp;           

	pitch	= (float)(angle[0] * M_PI/180); 
	yaw		= (float)(angle[1] * M_PI/180); 
	tmp		= (float) cos(pitch);

	vector[0] = (float) (-tmp * -cos(yaw)); 
	vector[1] = (float) (sin(yaw)*tmp);
	vector[2] = (float) -sin(pitch);
}

QAngle m_angle;
void cCAimbot::CalcAngle( Vector &src, Vector &dst, QAngle &angles )
{
	m_angle = angles;
	double delta[3] = { (src[0]-dst[0]), (src[1]-dst[1]), (src[2]-dst[2]) };
	double hyp = sqrt(delta[0]*delta[0] + delta[1]*delta[1]);

	angles[0] = (float) (atan(delta[2]/hyp) * M_RADPI);
	angles[1] = (float) (atan(delta[1]/delta[0]) * M_RADPI);
	angles[2] = 0.0f;

	if(delta[0] >= 0.0) { angles[1] += 180.0f; }
}

float cCAimbot::GetFov( QAngle angle, Vector src, Vector dst ) 
{ 
	QAngle ang,aim; 
	float fov; 

	CalcAngle(src, dst, ang); 
	MakeVector(angle, aim); 
	MakeVector(ang, ang);      

	float mag_s = sqrt((aim[0]*aim[0]) + (aim[1]*aim[1]) + (aim[2]*aim[2])); 
	float mag_d = sqrt((aim[0]*aim[0]) + (aim[1]*aim[1]) + (aim[2]*aim[2])); 

	float u_dot_v = aim[0]*ang[0] + aim[1]*ang[1] + aim[2]*ang[2]; 

	fov = acos(u_dot_v / (mag_s*mag_d)) * (180.0 / M_PI); 

	return fov; 
}
bool cCAimbot::GetVisible( Vector& vecAbsStart, Vector& vecAbsEnd, C_BaseEntity* pBaseEnt ) 
{ 
	player_info_t pinfo;
	trace_t tr;
	Ray_t ray;

	ray.Init( vecAbsStart, vecAbsEnd );
	VoidCSS.g_pEnginetrace->TraceRay( ray, MASK_NPCWORLDSTATIC|CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX, NULL, &tr );

	if ( tr.fraction > 0.97f )
		return true;

	if ( tr.m_pEnt && pBaseEnt )
	{
		if ( tr.m_pEnt->index == 0 || tr.allsolid )
			return false;

		if (( VoidCSS.g_pEngClient->GetPlayerInfo( tr.m_pEnt->index, &pinfo )
			|| pBaseEnt->index == tr.m_pEnt->index) && tr.fraction > 0.92)
			return true;
	}
	return false;
}

bool GetBonePosition ( int iBone, Vector& vecOrigin, QAngle qAngles, int index )
{
	if( iBone < 0 || iBone >= 20 )
		return false;

	matrix3x4_t pmatrix[MAXSTUDIOBONES];

	IClientEntity* ClientEntity = VoidCSS.g_pEntList->GetClientEntity( index );

	if ( ClientEntity == NULL )
		return false;

	if ( ClientEntity->IsDormant() )
		return false;

	IClientRenderable* RenderEntity =  ClientEntity->GetClientRenderable();

	if(	RenderEntity == NULL/* nothing */ )
		return false;

	if( RenderEntity->SetupBones( pmatrix, 128, BONE_USED_BY_HITBOX, VoidCSS.g_pGlobals->curtime ) == false )
		return false;

	MatrixAngles( pmatrix[ iBone ], qAngles, vecOrigin );

	return true;
}
void cCAimbot::AimAtTarget( CUserCmd* c )
{ 
	if( VoidCSS.g_pEngClient->IsInGame() == false 
	||	VoidCSS.g_pEngClient->IsHLTV() == true  
	||	VoidCSS.g_pMyPlayer->BaseEnt() == NULL )
		return;

	DropTarget();
	//----------------------------------//
	player_info_t pinfo;
	QAngle* pPunchAngle = ( QAngle* )( (DWORD)VoidCSS.g_pMyPlayer->BaseEnt() + 0xBB0 );
	//----------------------------------//
	//----------------------------------//
	for( int index = 1; index <= VoidCSS.g_pEntList->NumberOfEntities( false ); ++index )
	{
		if ( index == VoidCSS.g_pEngClient->GetLocalPlayer() )
			continue;

		IClientEntity* ClientEntity = VoidCSS.g_pEntList->GetClientEntity( index );

		if (	ClientEntity == NULL
			||	ClientEntity->IsDormant() )
				continue;

		CBaseEntity* pBaseEntity = ClientEntity->GetBaseEntity();
		//Get Life State
		int *lifestate = (int*) ( ( DWORD )pBaseEntity + ( DWORD )0x87 );
		float fov = (float)VoidCSS.s_aimfov;
		if (VoidCSS.s_assshot)
			fov = 360.0f;

		if(		pBaseEntity == NULL/* nothing */
			||	pBaseEntity->IsDormant()/* not active */
			||	!(*lifestate  == LIFE_ALIVE)/* not alive */
			||	VoidCSS.g_pEngClient->GetPlayerInfo( index, &pinfo ) == false/* not a player!*/
			||	pBaseEntity->GetTeamNumber() == VoidCSS.g_pMyPlayer->BaseEnt()->GetTeamNumber()/*enemy*/
			||	!GetBonePosition( 14/*Head*/, vPlayer, c->viewangles, index )
			||	GetFov( VoidCSS.g_pMyPlayer->BaseEnt()->GetAbsAngles(), VoidCSS.g_pMyPlayer->BaseEnt()->EyePosition(), vPlayer ) > fov /*4.0f*/
			||	!GetVisible( VoidCSS.g_pMyPlayer->BaseEnt()->EyePosition(), vPlayer, pBaseEntity )/* not visible */
			||	flBestDist < GetFov( VoidCSS.g_pMyPlayer->BaseEnt()->GetAbsAngles(), VoidCSS.g_pMyPlayer->BaseEnt()->EyePosition(), vPlayer ))
				continue;

		flBestDist = GetFov( VoidCSS.g_pMyPlayer->BaseEnt()->GetAbsAngles(), VoidCSS.g_pMyPlayer->BaseEnt()->EyePosition(), vPlayer );

		m_nTarget = index;
		// Calculate the delta origin
		Vector vDeltaOrigin = vPlayer - vPlayer;
		// Calculate the Latency
		float fLatency = VoidCSS.g_pEngClient->GetNetChannelInfo()->GetLatency( FLOW_OUTGOING );
		// Compensate the latency
		vDeltaOrigin[0] *= fLatency;
		vDeltaOrigin[1] *= fLatency;
		vDeltaOrigin[2] *= fLatency;
		//Apply the prediction
		PredictedTargetPosition = vPlayer + vDeltaOrigin;
	}
	if( m_nTarget == -1 )	
		return;

	CalcAngle( VoidCSS.g_pMyPlayer->BaseEnt()->EyePosition(), PredictedTargetPosition, c->viewangles );
	//recoil
	c->viewangles.x -= (	pPunchAngle->x *2.0f	);
	c->viewangles.y -= (	pPunchAngle->y *2.0f	);
	//aimbot
	VoidCSS.g_pEngClient->SetViewAngles( c->viewangles );//aim
	//nospread
	float flNew[3],flOld[3];
	flOld[0] = c->viewangles[0];
	flOld[1] = c->viewangles[1];
	flOld[2] = c->viewangles[2];
	//VoidCSS.g_pNoSpread->GetSpreadFix( c->random_seed, flOld, flNew );
	c->viewangles[0] += flNew[0];
	c->viewangles[1] += flNew[1];
	c->viewangles[2] += flNew[2];

	if (VoidCSS.s_assshot)
		VoidCSS.g_pEngClient->SetViewAngles( m_angle );

	DropTarget();
}