/*~--------------------------------------------------------------------------~*
 * Copyright (c) 2017 Los Alamos National Laboratory, LLC
 * All rights reserved
 *~--------------------------------------------------------------------------~*/
////////////////////////////////////////////////////////////////////////////////
/// \file
////////////////////////////////////////////////////////////////////////////////

#pragma once

// user includes
#include "flecsi-sp/burton/burton_mesh.h"



////////////////////////////////////////////////////////////////////////////////
// General Interface
////////////////////////////////////////////////////////////////////////////////

  
template< typename E >
auto filter_boundary( E && entities )  
{
  return 
    std::forward<decltype(entities)>(entities).filter( 
      [](auto e) { return e->is_boundary(); } 
    );
};


////////////////////////////////////////////////////////////////////////////////
// Alias mesh types
////////////////////////////////////////////////////////////////////////////////

namespace flecsi {
namespace sp {
namespace burton {

//! \brief The final 2d mesh type
using burton_mesh_2d_t = burton_mesh_t<2>;
//! \brief The final 3d mesh type
using burton_mesh_3d_t = burton_mesh_t<3>;

}
}
}
