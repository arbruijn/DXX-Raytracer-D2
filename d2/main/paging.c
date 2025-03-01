/*
THE COMPUTER CODE CONTAINED HEREIN IS THE SOLE PROPERTY OF PARALLAX
SOFTWARE CORPORATION ("PARALLAX").  PARALLAX, IN DISTRIBUTING THE CODE TO
END-USERS, AND SUBJECT TO ALL OF THE TERMS AND CONDITIONS HEREIN, GRANTS A
ROYALTY-FREE, PERPETUAL LICENSE TO SUCH END-USERS FOR USE BY SUCH END-USERS
IN USING, DISPLAYING,  AND CREATING DERIVATIVE WORKS THEREOF, SO LONG AS
SUCH USE, DISPLAY OR CREATION IS FOR NON-COMMERCIAL, ROYALTY OR REVENUE
FREE PURPOSES.  IN NO EVENT SHALL THE END-USER USE THE COMPUTER CODE
CONTAINED HEREIN FOR REVENUE-BEARING PURPOSES.  THE END-USER UNDERSTANDS
AND AGREES TO THE TERMS HEREIN AND ACCEPTS THE SAME BY USE OF THIS FILE.
COPYRIGHT 1993-1999 PARALLAX SOFTWARE CORPORATION.  ALL RIGHTS RESERVED.
*/

/*
 *
 * Routines for paging in/out textures.
 *
 */

//#define PSX_BUILD_TOOLS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "pstypes.h"
#include "inferno.h"
#include "segment.h"
#include "textures.h"
#include "wall.h"
#include "object.h"
#include "gamemine.h"
#include "dxxerror.h"
#include "gameseg.h"
#include "game.h"
#include "piggy.h"
#include "texmerge.h"
#include "polyobj.h"
#include "vclip.h"
#include "effects.h"
#include "fireball.h"
#include "weapon.h"
#include "palette.h"
#include "timer.h"
#include "text.h"
#include "cntrlcen.h"
#include "gauges.h"
#include "powerup.h"
#include "fuelcen.h"
#include "mission.h"
#include "args.h"
#include "ai.h"


void paging_touch_vclip( vclip * vc )
{
	int i;

	for (i=0; i<vc->num_frames; i++ )	{
		PIGGY_PAGE_IN( vc->frames[i] );
	}
}


void paging_touch_wall_effects( int tmap_num )
{
	int i;

	for (i=0;i<Num_effects;i++)	{
		if ( Effects[i].changing_wall_texture == tmap_num )	{
			paging_touch_vclip( &Effects[i].vc );

			if (Effects[i].dest_bm_num > -1)
				PIGGY_PAGE_IN( Textures[Effects[i].dest_bm_num] );	//use this bitmap when monitor destroyed
			if ( Effects[i].dest_vclip > -1 )
				paging_touch_vclip( &Vclip[Effects[i].dest_vclip] );		  //what vclip to play when exploding

			if ( Effects[i].dest_eclip > -1 )
				paging_touch_vclip( &Effects[Effects[i].dest_eclip].vc ); //what eclip to play when exploding

			if ( Effects[i].crit_clip > -1 )
				paging_touch_vclip( &Effects[Effects[i].crit_clip].vc ); //what eclip to play when mine critical
		}

	}

	if ( TmapInfo[tmap_num].destroyed != -1 )
		PIGGY_PAGE_IN( Textures[TmapInfo[tmap_num].destroyed] );
	if ( TmapInfo[tmap_num].flags & TMI_VOLATILE )
		paging_touch_vclip( &Vclip[VCLIP_VOLATILE_WALL_HIT] );
	if ( TmapInfo[tmap_num].flags & TMI_WATER )
		paging_touch_vclip( &Vclip[VCLIP_WATER_HIT] );
}

void paging_touch_object_effects( int tmap_num )
{
	int i;

	for (i=0;i<Num_effects;i++)	{
		if ( Effects[i].changing_object_texture == tmap_num )	{
			paging_touch_vclip( &Effects[i].vc );
		}
	}
}


void paging_touch_model( int modelnum )
{
	int i;
	polymodel *pm = &Polygon_models[modelnum];

	for (i=0;i<pm->n_textures;i++)	{
		PIGGY_PAGE_IN( ObjBitmaps[ObjBitmapPtrs[pm->first_texture+i]] );
		paging_touch_object_effects( ObjBitmapPtrs[pm->first_texture+i] );
		#ifdef PSX_BUILD_TOOLS
		// cmp added
		paging_touch_object_effects( pm->first_texture+i );
		#endif
	}
}

void paging_touch_weapon( int weapon_type )
{
	// Page in the robot's weapons.
	
	if ( (weapon_type < 0) || (weapon_type > N_weapon_types) ) return;

	if ( Weapon_info[weapon_type].picture.index )	{
		if ( HIRESMODE )
			PIGGY_PAGE_IN( Weapon_info[weapon_type].hires_picture );
		else
			PIGGY_PAGE_IN( Weapon_info[weapon_type].picture );
	}		
	
	if ( Weapon_info[weapon_type].flash_vclip > -1 )
		paging_touch_vclip(&Vclip[Weapon_info[weapon_type].flash_vclip]);
	if ( Weapon_info[weapon_type].wall_hit_vclip > -1 )
		paging_touch_vclip(&Vclip[Weapon_info[weapon_type].wall_hit_vclip]);
	if ( Weapon_info[weapon_type].damage_radius )	{
		// Robot_hit_vclips are actually badass_vclips
		if ( Weapon_info[weapon_type].robot_hit_vclip > -1 )
			paging_touch_vclip(&Vclip[Weapon_info[weapon_type].robot_hit_vclip]);
	}

	switch( Weapon_info[weapon_type].render_type )	{
	case WEAPON_RENDER_VCLIP:
		if ( Weapon_info[weapon_type].weapon_vclip > -1 )
			paging_touch_vclip( &Vclip[Weapon_info[weapon_type].weapon_vclip] );
		break;
	case WEAPON_RENDER_NONE:
		break;
	case WEAPON_RENDER_POLYMODEL:
		paging_touch_model( Weapon_info[weapon_type].model_num );
		break;
	case WEAPON_RENDER_BLOB:
		PIGGY_PAGE_IN( Weapon_info[weapon_type].bitmap );
		break;
	}
}



sbyte super_boss_gate_type_list[13] = {0, 1, 8, 9, 10, 11, 12, 15, 16, 18, 19, 20, 22 };

void paging_touch_robot( int robot_index )
{
	int i;

	// Page in robot_index
	paging_touch_model(Robot_info[robot_index].model_num);
	if ( Robot_info[robot_index].exp1_vclip_num>-1 )
		paging_touch_vclip(&Vclip[Robot_info[robot_index].exp1_vclip_num]);
	if ( Robot_info[robot_index].exp2_vclip_num>-1 )
		paging_touch_vclip(&Vclip[Robot_info[robot_index].exp2_vclip_num]);

	// Page in his weapons
	paging_touch_weapon( Robot_info[robot_index].weapon_type );

	// A super-boss can gate in robots...
	if ( Robot_info[robot_index].boss_flag>=2 )	{
		int boss = Robot_info[robot_index].boss_flag - BOSS_D2;
		extern Spew_bots[NUM_D2_BOSSES][3];
		if (boss >= 0 && boss < NUM_D2_BOSSES) {
			for (i=0; i<3; i++ )
				if (Spew_bots[boss][i] != -1)
					paging_touch_robot(Spew_bots[boss][i]);
		} else
			for (i=0; i<13; i++ )
				paging_touch_robot(super_boss_gate_type_list[i]);

		paging_touch_vclip( &Vclip[VCLIP_MORPHING_ROBOT] );
	}
}


void paging_touch_object( object * obj )
{
	int v;

	switch (obj->render_type) {

		case RT_NONE:	break;		//doesn't render, like the player

		case RT_POLYOBJ:
			if ( obj->rtype.pobj_info.tmap_override != -1 )
				PIGGY_PAGE_IN( Textures[obj->rtype.pobj_info.tmap_override] );
			else
				paging_touch_model(obj->rtype.pobj_info.model_num);
			break;

		case RT_POWERUP:
			if ( obj->rtype.vclip_info.vclip_num > -1 ) {
				paging_touch_vclip(&Vclip[obj->rtype.vclip_info.vclip_num]);
			}
			break;

		case RT_MORPH:	break;

		case RT_FIREBALL: break;

		case RT_WEAPON_VCLIP: break;

		case RT_HOSTAGE:
			paging_touch_vclip(&Vclip[obj->rtype.vclip_info.vclip_num]);
			break;

		case RT_LASER: break;
 	}

	switch (obj->type) {	
	case OBJ_PLAYER:	
		v = get_explosion_vclip(obj, 0);
		if ( v > -1 )
			paging_touch_vclip(&Vclip[v]);
		break;
	case OBJ_ROBOT:
		paging_touch_robot( obj->id );
		break;
	case OBJ_CNTRLCEN:
		paging_touch_weapon( CONTROLCEN_WEAPON_NUM );
		if (Dead_modelnums[obj->rtype.pobj_info.model_num] != -1)	{
			paging_touch_model( Dead_modelnums[obj->rtype.pobj_info.model_num] );
		}
		break;
	}
}

	

void paging_touch_side( segment * segp, int sidenum )
{
	int tmap1, tmap2;

	//if (!(WALL_IS_DOORWAY(segp,sidenum) & WID_RENDER_FLAG))
	if (segp->children[sidenum] != -1 && segp->sides[sidenum].wall_num == -1)
		return;
	
	tmap1 = segp->sides[sidenum].tmap_num;
	paging_touch_wall_effects(tmap1);
	tmap2 = segp->sides[sidenum].tmap_num2;
	PIGGY_PAGE_IN( Textures[tmap1] );
	if (tmap2 != 0)	{
		PIGGY_PAGE_IN( Textures[tmap2 & 0x3FFF] );
		paging_touch_wall_effects( tmap2 & 0x3FFF );
	}

	// PSX STUFF
	#ifdef PSX_BUILD_TOOLS
		// If there is water on the level, then force the water splash into memory
		if(!(TmapInfo[tmap1].flags & TMI_VOLATILE) && (TmapInfo[tmap1].flags & TMI_WATER)) {
			bitmap_index Splash;
			Splash.index = 1098;
			PIGGY_PAGE_IN(Splash);
			Splash.index = 1099;
			PIGGY_PAGE_IN(Splash);
			Splash.index = 1100;
			PIGGY_PAGE_IN(Splash);
			Splash.index = 1101;
			PIGGY_PAGE_IN(Splash);
			Splash.index = 1102;
			PIGGY_PAGE_IN(Splash);
		}
	#endif

}

void paging_touch_robot_maker( segment * segp )
{
	segment2	*seg2p = &Segment2s[segp-Segments];

	if ( seg2p->special == SEGMENT_IS_ROBOTMAKER )	{
		paging_touch_vclip(&Vclip[VCLIP_MORPHING_ROBOT]);
			int	i;
			uint	flags;
			int	robot_index;

			for (i=0;i<2;i++) {
				robot_index = i*32;
				flags = RobotCenters[seg2p->matcen_num].robot_flags[i];
				while (flags) {
					if (flags & 1)	{
						// Page in robot_index
						paging_touch_robot( robot_index );
					}
					flags >>= 1;
					robot_index++;
				}
			}
	}
}


void paging_touch_segment(segment * segp)
{
	int sn;
	int objnum;
	segment2	*seg2p = &Segment2s[segp-Segments];

	if ( seg2p->special == SEGMENT_IS_ROBOTMAKER )	
		paging_touch_robot_maker(segp);

//	paging_draw_orb();
	for (sn=0;sn<MAX_SIDES_PER_SEGMENT;sn++) {
//		paging_draw_orb();
		paging_touch_side( segp, sn );
	}

	for (objnum=segp->objects;objnum!=-1;objnum=Objects[objnum].next)	{
//		paging_draw_orb();
		paging_touch_object( &Objects[objnum] );
	}
}



void paging_touch_walls()
{
	int i,j;
	wclip *anim;

	for (i=0;i<Num_walls;i++) {
//		paging_draw_orb();
		if ( Walls[i].clip_num > -1 )	{
			anim = &WallAnims[Walls[i].clip_num];
			for (j=0; j < anim->num_frames; j++ )	{
				PIGGY_PAGE_IN( Textures[anim->frames[j]] );
			}
		}
	}
}

void paging_touch_all()
{
	int s;
	
	stop_time();

	for (s=0; s<=Highest_segment_index; s++)	{
		paging_touch_segment( &Segments[s] );
	}	
	paging_touch_walls();

	for ( s=0; s < N_powerup_types; s++ )	{
		if ( Powerup_info[s].vclip_num > -1 )	
			paging_touch_vclip(&Vclip[Powerup_info[s].vclip_num]);
	}

	for ( s=0; s<N_weapon_types; s++ )	{
		paging_touch_weapon(s);
	}

	for ( s=0; s < N_powerup_types; s++ )	{
		if ( Powerup_info[s].vclip_num > -1 )	
			paging_touch_vclip(&Vclip[Powerup_info[s].vclip_num]);
	}

	if ( HIRESMODE )	{
		for (s=0; s<MAX_GAUGE_BMS; s++ )	{
			if ( Gauges_hires[s].index )
				PIGGY_PAGE_IN( Gauges_hires[s] );
		}
		for (s=0; s<Num_cockpits/2; s++ )	{
			PIGGY_PAGE_IN( cockpit_bitmap[s+Num_cockpits/2] );
		}
	} else {
		for (s=0; s<MAX_GAUGE_BMS; s++ )	{
			if ( Gauges[s].index )
				PIGGY_PAGE_IN( Gauges[s] );
		}
		for (s=0; s<Num_cockpits/2; s++ )	{
			PIGGY_PAGE_IN( cockpit_bitmap[s] );
		}
	}
	paging_touch_vclip( &Vclip[VCLIP_PLAYER_APPEARANCE] );
	paging_touch_vclip( &Vclip[VCLIP_POWERUP_DISAPPEARANCE] );
	paging_touch_vclip( &Vclip[VCLIP_MONITOR_STATIC] );

	paging_touch_model(Marker_model_num);

#ifdef PSX_BUILD_TOOLS

	//PSX STUFF
	paging_touch_walls();
	for(s=0; s<=Highest_object_index; s++) {
		paging_touch_object(&Objects[s]);
	}


	{
		char * p;
		extern int Current_level_num;
		extern ushort GameBitmapXlat[MAX_BITMAP_FILES];
		short Used[MAX_BITMAP_FILES];
		PHYSFS_file * fp;
		char fname[128];
		int i;

		if (Current_level_num<0)                //secret level
			strcpy( fname, Secret_level_names[-Current_level_num-1] );
		else                                    //normal level
			strcpy( fname, Level_names[Current_level_num-1] );
		p = strchr( fname, '.' );
		if ( p ) *p = 0;
		strcat( fname, ".pag" );

		fp = PHYSFSX_openWriteBuffered( fname );
		for (i=0; i<MAX_BITMAP_FILES;i++ ) {
			Used[i] = 0;
		}
		for (i=0; i<MAX_BITMAP_FILES;i++ ) {
			Used[GameBitmapXlat[i]]++;
		}

		//cmp added so that .damage bitmaps are included for paged-in lights of the current level
		for (i=0; i<MAX_TEXTURES;i++) {
			if(Textures[i].index > 0 && Textures[i].index < MAX_BITMAP_FILES &&
				Used[Textures[i].index] > 0 &&
				TmapInfo[i].destroyed > 0 && TmapInfo[i].destroyed < MAX_BITMAP_FILES) {
				Used[Textures[TmapInfo[i].destroyed].index] += 1;
				PIGGY_PAGE_IN(Textures[TmapInfo[i].destroyed]);

			}
		}

		//	Force cockpit to be paged in.
		{
			bitmap_index bonk;
			bonk.index = 109;
			PIGGY_PAGE_IN(bonk);
		}

		// Force in the frames for markers
		{
			bitmap_index bonk2;
			bonk2.index = 2014;
			PIGGY_PAGE_IN(bonk2);
			bonk2.index = 2015;
			PIGGY_PAGE_IN(bonk2);
			bonk2.index = 2016;
			PIGGY_PAGE_IN(bonk2);
			bonk2.index = 2017;
			PIGGY_PAGE_IN(bonk2);
			bonk2.index = 2018;
			PIGGY_PAGE_IN(bonk2);
		}

		for (i=0; i<MAX_BITMAP_FILES;i++ ) {
			int paged_in = 1;
			// cmp debug
			//piggy_get_bitmap_name(i,fname);

			if (GameBitmaps[i].bm_flags & BM_FLAG_PAGED_OUT)
				paged_in = 0;

//                      if (GameBitmapXlat[i]!=i)
//                              paged_in = 0;

			if ( !Used[i] )
				paged_in = 0;
			if ( (i==47) || (i==48) )               // Mark red mplayer ship textures as paged in.
				paged_in = 1;
	
			if ( !paged_in )
				PHYSFSX_printf( fp, "0,\t// Bitmap %d (%s)\n", i, "test\0"); // cmp debug fname );
			else
				PHYSFSX_printf( fp, "1,\t// Bitmap %d (%s)\n", i, "test\0"); // cmp debug fname );
		}

		PHYSFS_close(fp);
	}
#endif

	start_time();
	reset_cockpit();		//force cockpit redraw next time
}

