/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on the 
 * source.
 *
*/



#ifndef _AIGOALS_H
#define _AIGOALS_H

#include "ai/ai_flags.h"
#include "globalincs/globals.h"
#include "globalincs/pstypes.h"
#include "parse/sexp.h"
#include "scripting/lua/LuaTypes.h"
#include "scripting/lua/LuaValue.h"

struct wing;
struct ai_info;

// macros for goals which get set via sexpressions in the mission code

// types of ai goals -- tyese types will help us to determination on which goals should
// have priority over others (i.e. when a player issues a goal to a wing, then a seperate
// goal to a ship in that wing).  We would probably use this type in conjunction with
// goal priority to establish which goal to follow
#define AIG_TYPE_EVENT_SHIP			1		// from mission event direct to ship
#define AIG_TYPE_EVENT_WING			2		// from mission event direct to wing
#define AIG_TYPE_PLAYER_SHIP		3		// from player direct to ship
#define AIG_TYPE_PLAYER_WING		4		// from player direct to wing
#define AIG_TYPE_DYNAMIC			5		// created on the fly

#define AI_GOAL_NONE				-1

// IMPORTANT!  If you add a new AI_GOAL_x define, be sure to update the functions
// ai_update_goal_references() and query_referenced_in_ai_goals() or else risk breaking
// Fred.  If the goal you add doesn't have a target (such as chase_any), then you don't have
// to worry about doing this.  Also add it to list in Fred\Management.cpp, and let Hoffoss know!
// WMC - Oh and add them to Ai_goal_names plz. TY! :)
// Goober5000 - As well as Ai_goal_text and Ai_goal_list, if appropriate
#define AI_GOAL_CHASE					(1<<1)	// 0x00000002
#define AI_GOAL_DOCK					(1<<2)	// 0x00000004	// used for undocking as well
#define AI_GOAL_WAYPOINTS				(1<<3)	// 0x00000008
#define AI_GOAL_WAYPOINTS_ONCE			(1<<4)	// 0x00000010
#define AI_GOAL_WARP					(1<<5)	// 0x00000020
#define AI_GOAL_DESTROY_SUBSYSTEM		(1<<6)	// 0x00000040
#define AI_GOAL_FORM_ON_WING			(1<<7)	// 0x00000080
#define AI_GOAL_UNDOCK					(1<<8)	// 0x00000100
#define AI_GOAL_CHASE_WING				(1<<9)	// 0x00000200
#define AI_GOAL_GUARD					(1<<10)	// 0x00000400
#define AI_GOAL_DISABLE_SHIP			(1<<11)	// 0x00000800
#define AI_GOAL_DISARM_SHIP				(1<<12)	// 0x00001000
#define AI_GOAL_CHASE_ANY				(1<<13)	// 0x00002000
#define AI_GOAL_IGNORE					(1<<14)	// 0x00004000
#define AI_GOAL_GUARD_WING				(1<<15)	// 0x00008000
#define AI_GOAL_EVADE_SHIP				(1<<16)	// 0x00010000

// the next goals are for support ships only
#define AI_GOAL_STAY_NEAR_SHIP			(1<<17)	// 0x00020000
#define AI_GOAL_KEEP_SAFE_DISTANCE		(1<<18)	// 0x00040000
#define AI_GOAL_REARM_REPAIR			(1<<19)	// 0x00080000

// resume regular goals
#define AI_GOAL_STAY_STILL				(1<<20)	// 0x00100000
#define AI_GOAL_PLAY_DEAD				(1<<21)	// 0x00200000

// added by SCP
#define AI_GOAL_CHASE_WEAPON			(1<<22)	// 0x00400000
#define AI_GOAL_FLY_TO_SHIP				(1<<23) // 0x00800000
#define AI_GOAL_IGNORE_NEW				(1<<24)	// 0x01000000
#define AI_GOAL_CHASE_SHIP_CLASS		(1<<25)	// 0x02000000
#define AI_GOAL_PLAY_DEAD_PERSISTENT	(1<<26)	// 0x03000000
#define AI_GOAL_LUA						(1<<27) // 0x04000000

// now the masks for ship types

// Goober5000: added AI_GOAL_STAY_NEAR_SHIP and AI_GOAL_KEEP_SAFE_DISTANCE as valid for fighters
//WMC - Don't need these anymore. Whee!
/*
#define AI_GOAL_ACCEPT_FIGHTER		( AI_GOAL_FLY_TO_SHIP | AI_GOAL_CHASE | AI_GOAL_WAYPOINTS | AI_GOAL_WAYPOINTS_ONCE | AI_GOAL_WARP | AI_GOAL_DESTROY_SUBSYSTEM | AI_GOAL_CHASE_WING | AI_GOAL_GUARD | AI_GOAL_DISABLE_SHIP | AI_GOAL_DISARM_SHIP | AI_GOAL_CHASE_ANY | AI_GOAL_IGNORE | AI_GOAL_IGNORE_NEW | AI_GOAL_GUARD_WING | AI_GOAL_EVADE_SHIP | AI_GOAL_STAY_STILL | AI_GOAL_PLAY_DEAD | AI_GOAL_PLAY_DEAD_PERSISTENT | AI_GOAL_STAY_NEAR_SHIP | AI_GOAL_KEEP_SAFE_DISTANCE )
#define AI_GOAL_ACCEPT_BOMBER			( AI_GOAL_FLY_TO_SHIP | AI_GOAL_ACCEPT_FIGHTER | AI_GOAL_STAY_NEAR_SHIP )
#define AI_GOAL_ACCEPT_STEALTH		( AI_GOAL_FLY_TO_SHIP | AI_GOAL_ACCEPT_FIGHTER | AI_GOAL_STAY_NEAR_SHIP )
#define AI_GOAL_ACCEPT_TRANSPORT		( AI_GOAL_FLY_TO_SHIP | AI_GOAL_CHASE | AI_GOAL_CHASE_WING | AI_GOAL_DOCK | AI_GOAL_WAYPOINTS | AI_GOAL_WAYPOINTS_ONCE | AI_GOAL_WARP | AI_GOAL_UNDOCK | AI_GOAL_STAY_STILL | AI_GOAL_PLAY_DEAD | AI_GOAL_PLAY_DEAD_PERSISTENT | AI_GOAL_STAY_NEAR_SHIP )
#define AI_GOAL_ACCEPT_FREIGHTER		( AI_GOAL_FLY_TO_SHIP | AI_GOAL_ACCEPT_TRANSPORT | AI_GOAL_STAY_NEAR_SHIP )
#define AI_GOAL_ACCEPT_CRUISER		( AI_GOAL_FLY_TO_SHIP | AI_GOAL_ACCEPT_FREIGHTER | AI_GOAL_STAY_NEAR_SHIP )
#define AI_GOAL_ACCEPT_CORVETTE		( AI_GOAL_FLY_TO_SHIP | AI_GOAL_ACCEPT_CRUISER | AI_GOAL_STAY_NEAR_SHIP )
#define AI_GOAL_ACCEPT_GAS_MINER		( AI_GOAL_FLY_TO_SHIP | AI_GOAL_ACCEPT_CRUISER | AI_GOAL_STAY_NEAR_SHIP )
#define AI_GOAL_ACCEPT_AWACS			( AI_GOAL_FLY_TO_SHIP | AI_GOAL_ACCEPT_CRUISER | AI_GOAL_STAY_NEAR_SHIP )
#define AI_GOAL_ACCEPT_CAPITAL		( AI_GOAL_FLY_TO_SHIP | AI_GOAL_ACCEPT_CRUISER & ~(AI_GOAL_DOCK | AI_GOAL_UNDOCK) | AI_GOAL_STAY_NEAR_SHIP )
#define AI_GOAL_ACCEPT_SUPERCAP		( AI_GOAL_FLY_TO_SHIP | AI_GOAL_ACCEPT_CAPITAL | AI_GOAL_STAY_NEAR_SHIP )
#define AI_GOAL_ACCEPT_SUPPORT		( AI_GOAL_FLY_TO_SHIP | AI_GOAL_DOCK | AI_GOAL_UNDOCK | AI_GOAL_WAYPOINTS | AI_GOAL_WAYPOINTS_ONCE | AI_GOAL_STAY_NEAR_SHIP | AI_GOAL_KEEP_SAFE_DISTANCE | AI_GOAL_STAY_STILL | AI_GOAL_PLAY_DEAD | AI_GOAL_PLAY_DEAD_PERSISTENT )
#define AI_GOAL_ACCEPT_ESCAPEPOD		( AI_GOAL_FLY_TO_SHIP | AI_GOAL_ACCEPT_TRANSPORT| AI_GOAL_STAY_NEAR_SHIP  )
*/

#define MAX_AI_DOCK_NAMES				25

enum class ai_achievability { ACHIEVABLE, NOT_ACHIEVABLE, NOT_KNOWN, SATISFIED };

struct ai_lua_parameters {
	object_ship_wing_point_team target;
	luacpp::LuaValueList arguments;
};

// structure for AI goals
typedef struct ai_goal {
	int	signature;			//	Unique identifier.  All goals ever created (per mission) have a unique signature.
	int	ai_mode;				// one of the AIM_* modes for this goal
	int	ai_submode;			// maybe need a submode
	int	type;					// one of the AIG_TYPE_* values above
	flagset<AI::Goal_Flags>	flags;				// one of the AIGF_* values above
	fix	time;					// time at which this goal was issued.
	int	priority;			// how important is this goal -- number 0 - 100

	const char *target_name;	// name of the thing that this goal acts upon
	int		target_name_index;	// index of goal_target_name in Goal_target_names[][]
	waypoint_list *wp_list;		// waypoints that this ship might fly.
	int target_instance;		// instance of thing this ship might be chasing (currently only used for weapons; note, not the same as objnum!)
	int	target_signature;		// signature of object this ship might be chasing (currently only used for weapons; paired with above value to confirm target)

	// extra goal-specific data
	int int_data;
	float float_data;

	// unions for docking stuff.
	// (AIGF_DOCKER_INDEX_VALID and AIGF_DOCKEE_INDEX_VALID tell us to use indexes; otherwise we use names)
	// these are the dockpoints used on the docker and dockee ships, not the ships themselves
	union {
		const char *name;
		int	index;
	} docker;
	
	union {
		const char *name;
		int	index;
	} dockee;

	ai_lua_parameters lua_ai_target;

} ai_goal;

extern void ai_goal_reset(ai_goal *aigp, bool adding_goal = false, int ai_mode = AI_GOAL_NONE, int ai_submode = -1, int type = -1);


typedef flag_def_list ai_goal_list;

extern ai_goal_list Ai_goal_names[];
extern int Num_ai_goals;

extern int Num_ai_dock_names;
extern char Ai_dock_names[MAX_AI_DOCK_NAMES][NAME_LENGTH];

extern const char *Ai_goal_text(int goal, int submode);

// every goal in a mission gets a unique signature
extern int Ai_goal_signature;

// extern function definitions
extern void ai_post_process_mission();
extern void ai_maybe_add_form_goal( wing *wingp );
extern void ai_process_mission_orders( int objnum, ai_info *aip );

extern int ai_goal_num(ai_goal *goals);

// adds goals to ships/wing through sexpressions
extern void ai_add_ship_goal_scripting(int mode, int submode, int priority, char *shipname, ai_info *aip);
extern void ai_add_ship_goal_sexp( int sexp, int type, ai_info *aip );
extern void ai_add_wing_goal_sexp( int sexp, int type, wing *wingp );
extern void ai_add_goal_sub_sexp( int sexp, int type, ai_info *aip, ai_goal *aigp, char *actor_name);

extern int ai_remove_goal_sexp_sub( int sexp, ai_goal* aigp );
extern void ai_remove_wing_goal_sexp( int sexp, wing *wingp );

// adds goals to ships/sings through player orders
extern void ai_add_ship_goal_player(int type, int mode, int submode, char* shipname, ai_info* aip, const ai_lua_parameters& lua_target = { object_ship_wing_point_team(), luacpp::LuaValueList{} });
extern void ai_add_wing_goal_player(int type, int mode, int submode, char* shipname, int wingnum, const ai_lua_parameters& lua_target = { object_ship_wing_point_team(), luacpp::LuaValueList{} });

extern void ai_remove_ship_goal( ai_info *aip, int index );
extern void ai_clear_ship_goals( ai_info *aip );
extern void ai_clear_wing_goals( wing *wingp );

extern void ai_copy_mission_wing_goal( ai_goal *aigp, ai_info *aip );

extern void ai_mission_goal_complete( ai_info *aip );
extern void ai_mission_wing_goal_complete( int wingnum, ai_goal *remove_goalp );

extern void ai_update_goal_references(ai_goal *goals, sexp_ref_type type, const char *old_name, const char *new_name);
extern bool query_referenced_in_ai_goals(ai_goal *goals, sexp_ref_type type, const char *name);
extern char *ai_add_dock_name(const char *str);

extern int ai_query_goal_valid( int ship, int ai_goal_type );

extern void ai_add_goal_ship_internal( ai_info *aip, int goal_type, char *name, int docker_point, int dockee_point, int immediate = 1 );

#endif
