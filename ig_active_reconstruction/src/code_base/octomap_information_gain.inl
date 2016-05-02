/* Copyright (c) 2015, Stefan Isler, islerstefan@bluewin.ch
*
This file is part of ig_active_reconstruction, a ROS package for...well,

ig_active_reconstruction is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
ig_active_reconstruction is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public License
along with ig_active_reconstruction. If not, see <http://www.gnu.org/licenses/>.
*/

#define TEMPT template<class TREE_TYPE>
#define CSCOPE InformationGain<TREE_TYPE>

namespace ig_active_reconstruction
{
  
namespace world_representation
{
  
namespace octomap
{  
  TEMPT
  CSCOPE::Utils::Config::Config()
  : p_unknown_prior_(0.5)
  , p_unknown_upper_bound_(0.8)
  , p_unknown_lower_bound_(0.2)
  , voxels_in_void_ray_(100)
  {
    
  }
  
  TEMPT
  double CSCOPE::Utils::entropy( typename TREE_TYPE::NodeType* voxel )
  {
    double occupancy = pOccupancy(voxel);
    return entropy(occupancy);
  }
  
  TEMPT
  double CSCOPE::Utils::entropy( double likelihood )
  {
    double p_free = 1-likelihood;
    if(likelihood==0 || p_free==0)
    {
	return 0;
    }
    double vox_ig = -likelihood*log(likelihood)-p_free*log(p_free);
    return vox_ig;
  }
  
  TEMPT
  bool CSCOPE::Utils::isUnknown( typename TREE_TYPE::NodeType* voxel)
  {
    return isUnknown( pOccupancy(voxel) );
  }
  
  TEMPT
  bool CSCOPE::Utils::isUnknown( double likelihood)
  {
    return likelihood<=config.p_unknown_upper_bound_ && likelihood>=config.p_unknown_lower_bound_;
  }
  
  TEMPT
  bool CSCOPE::Utils::isOccupied( typename TREE_TYPE::NodeType* voxel )
  {
    return isOccupied( pOccupancy(voxel) );
  }
  
  TEMPT
  bool CSCOPE::Utils::isOccupied( double likelihood )
  {
    return likelihood>config.p_unknown_upper_bound_;
  }
  
  TEMPT
  bool CSCOPE::Utils::isFree( typename TREE_TYPE::NodeType* voxel )
  {
    return isFree( pOccupancy(voxel) );
  }
  
  TEMPT
  bool CSCOPE::Utils::isFree( double likelihood )
  {
    return likelihood<config.p_unknown_lower_bound_;
  }
  
  TEMPT
  double CSCOPE::Utils::pOccupancy( typename TREE_TYPE::NodeType* voxel )
  {
    double p_occ;
    
    if( voxel==NULL )
    {
      p_occ=config.p_unknown_prior_; // default for unknown
    }
    else if( !voxel->hasMeasurement() )
    {
	p_occ=config.p_unknown_prior_; // default for unknown
    }
    else
    {
      p_occ = voxel->getOccupancy();
    }
    return p_occ;
  }
}

}

}

#undef CSCOPE
#undef TEMPT