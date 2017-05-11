/*~--------------------------------------------------------------------------~*
 * Copyright (c) 2015 Los Alamos National Security, LLC
 * All rights reserved.
 *~--------------------------------------------------------------------------~*/

#ifndef flecsi_sp_minimal_entity_types_h
#define flecsi_sp_minimal_entity_types_h

#include <flecsi/topology/mesh_types.h>

#include "flecsi-sp/minimal/config.h"

///
// \file minimal_entity_types.h
// \authors bergen bird
// \date Initial file creation: Oct 19, 2016
///

namespace flecsi {
namespace sp {

///
// \struct minimal_vertex_t
// \brief FIXME
///
struct minimal_vertex_t
  : public topology::mesh_entity_t<0,
    minimal_config_t::num_domains>
{
  using point_t = minimal_config_t::point_t;

  ///
  // Constructor.
  //
  // \param mesh FIXME
  ///
  minimal_vertex_t(
    topology::mesh_topology_base_t & mesh,
    const point_t & coordinates
  )
    : mesh_(mesh), coordinates_(coordinates)
  {
  }

  const point_t &
  coordinates()
  const
  {
    return coordinates_;
  } // coordinates

private:

  topology::mesh_topology_base_t & mesh_;
  point_t coordinates_;

}; // class minimal_vertex_t

///
// \struct minimal_edge_t
// \breif FIXME
///
struct minimal_edge_t
  : public topology::mesh_entity_t<1,
    minimal_config_t::num_domains>
{
  ///
  // Constructor.
  //
  // \param mesh FIXME
  ///
  minimal_edge_t(topology::mesh_topology_base_t & mesh)
    : mesh_(mesh) {}

private:

  topology::mesh_topology_base_t & mesh_;

}; // class minimal_edge_t

#if FLECSI_MESH_DIMENSION == 3

///
// \struct minimal_face_t
// \breif FIXME
///
struct minimal_face_t
  : public topology::mesh_entity_t<2,
    minimal_config_t::num_domains>
{
  ///
  // Constructor.
  //
  // \param mesh FIXME
  ///
  minimal_face_t(topology::mesh_topology_base_t & mesh)
    : mesh_(mesh) {}

private:

  topology::mesh_topology_base_t & mesh_;

}; // class minimal_face_t

#endif // FLECSI_MESH_DIMENSION

enum class cell_type_t : size_t {
  unknown,
  domain_boundary
}; // enum cell_type_t

///
// \struct minimal_cell_t
// \breif FIXME
///
struct minimal_cell_t
  : public topology::mesh_entity_t<FLECSI_MESH_DIMENSION,
    minimal_config_t::num_domains>
{
  using real_t = minimal_config_t::real_t;

  ///
  // Constructor.
  //
  // \param mesh FIXME
  ///
  minimal_cell_t(topology::mesh_topology_base_t & mesh, cell_type_t type)
    : mesh_(mesh), type_(type) {}

  /**
   * @brief Informally, flecsi does not do the associate of higher order
   * things (edges/faces). This function describes these associations, for
   * example which vertices are associated with which edges (or faces).
   *
   * @param cell_id Id of this cell
   * @param dim Dimension
   * @param c Connectivity
   * @param e Edges
   *
   * @return Returns the number of vertices per edge described
   */
  std::vector<size_t>
  create_entities(
    id_t cell_id,
    size_t dim,
    topology::domain_connectivity<FLECSI_MESH_DIMENSION> & c,
    id_t * e
  )
  {
    id_t* v = c.get_entities(cell_id, 0);

    e[0] = v[0];
    e[1] = v[2];

    e[2] = v[1];
    e[3] = v[3];

    e[4] = v[0];
    e[5] = v[1];

    e[6] = v[2];
    e[7] = v[3];

    return {2, 2, 2, 2};
  } // create_entities

  real_t
  volume()
  {
    // FIXME
    return 1.0;
  } // volume

  cell_type_t
  type()
  {
    return type_;
  } // type

private:

  topology::mesh_topology_base_t & mesh_;
  cell_type_t type_;

}; // class minimal_cell_t

} // namespace sp
} // namespace flecsi

#endif // flecsi_sp_minimal_entity_types_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options for vim.
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
