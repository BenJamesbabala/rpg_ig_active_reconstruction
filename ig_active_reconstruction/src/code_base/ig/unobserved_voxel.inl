/* Copyright (c) 2015, Stefan Isler, islerstefan@bluewin.ch
*
This file is part of ig_active_reconstruction, a ROS package for...well,

ig_active_reconstruction is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
ig_active_reconstruction is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY
  {
    
  }
  
  TEMPT without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public License
along with ig_active_reconstruction. If not, see <http://www.gnu.org/licenses/>.
*/

#define TEMPT template<class TREE_TYPE>
#define CSCOPE UnobservedVoxelIg<TREE_TYPE>


namespace ig_active_reconstruction
{
  
namespace world_representation
{
  
namespace octomap
{  
  TEMPT
  CSCOPE::UnobservedVoxelIg( Utils utils )
  : utils_(utils)
  , ig_(0)
  , p_vis_(1)
  , voxel_count_(0)
  {
    
  }
  
  TEMPT
  std::string CSCOPE::type()
  {
    return "UnobservedVoxelIg";
  }
  
  TEMPT
  typename CSCOPE::GainType CSCOPE::getInformation()
  {
    return ig_;
  }
  
  TEMPT
  void CSCOPE::makeReadyForNewRay()
  {
    p_vis_=1;
  }
  
  TEMPT
  void CSCOPE::reset()
  {
    ig_ = 0;
    p_vis_ = 1;
    voxel_count_ = 0;
  }
  
  TEMPT
  void CSCOPE::includeRayMeasurement( typename TREE_TYPE::NodeType* node )
  {
    includeMeasurement(node);
  }
  
  TEMPT
  void CSCOPE::includeEndPointMeasurement( typename TREE_TYPE::NodeType* node )
  {
    includeMeasurement(node);
  }
  
  TEMPT
  void CSCOPE::informAboutVoidRay()
  {
    voxel_count_ += utils_.config.voxels_in_void_ray_;
    // no approximation needed, can be exactly calculated using the geometric series formula
    ig_ += utils_.entropy(utils_.config.p_unknown_prior_)/(1-utils_.config.p_unknown_prior_); // information in void ray...
  }
  
  TEMPT
  uint64_t CSCOPE::voxelCount()
  {
    return voxel_count_;
  }
  
  TEMPT
  void CSCOPE::includeMeasurement( typename TREE_TYPE::NodeType* node )
  {
    double p_occ = utils_.pOccupancy(node);
    
    if( utils_.isUnknown(p_occ) )
    {
      ++voxel_count_;
      double vox_ent = utils_.entropy(p_occ);
      ig_ += p_vis_*vox_ent;
    }
    
    p_vis_ *= p_occ;
  }
  
}

}

}

#undef CSCOPE
#undef TEMPT