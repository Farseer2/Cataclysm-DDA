#include "level_cache.h"

#include <algorithm>

static_assert( static_cast<int>( lit_level::DARK ) == 0,
               "visibility_cache won't be default initialized to 0" );

level_cache::level_cache()
{
    transparency_cache_dirty.set();
}

bool level_cache::get_veh_in_active_range() const
{
    return !veh_cached_parts.empty();
}

bool level_cache::get_veh_exists_at( const tripoint &pt ) const
{
    return veh_exists_at[ pt.x * MAPSIZE_X + pt.y ];
}

std::pair<vehicle *, int> level_cache::get_veh_cached_parts( const tripoint &pt ) const
{
    auto it = veh_cached_parts.find( pt );
    if( it != veh_cached_parts.end() ) {
        return it->second;
    }
    vehicle *veh = nullptr;
    return std::make_pair( veh, -1 );
}

void level_cache::set_veh_exists_at( const tripoint &pt, bool exists_at )
{
    veh_cache_cleared = false;
    veh_exists_at[ pt.x * MAPSIZE_X + pt.y ] = exists_at;
}

void level_cache::set_veh_cached_parts( const tripoint &pt, vehicle &veh, int part_num )
{
    veh_cache_cleared = false;
    veh_cached_parts[ pt ] = std::make_pair( &veh, part_num );
}

void level_cache::clear_vehicle_cache()
{
    if( veh_cache_cleared ) {
        return;
    }
    veh_exists_at.reset();
    veh_cached_parts.clear();
    veh_cache_cleared = true;
}

void level_cache::clear_veh_from_veh_cached_parts( const tripoint &pt, vehicle *veh )
{
    auto it = veh_cached_parts.find( pt );
    if( it != veh_cached_parts.end() && it->second.first == veh ) {
        veh_cached_parts.erase( it );
    }
}
