/*~--------------------------------------------------------------------------~*
 * Copyright (c) 2016 Los Alamos National Laboratory, LLC
 * All rights reserved
 *~--------------------------------------------------------------------------~*/

///////////////////////////// TODO //////////////////////////////////
//
// Particle shape?
// boundary style things? periodic? 
// data layout? method to load particle properties? Not married to AoS?
// replace all locals (eg nx) with assignment _globals (eg NX_global)
// Add support for multiple species
// Add an automate-able validation 

///////////////////////////// END TODO //////////////////////////////////

#ifndef driver_h
#define driver_h

// Includes 
#include <iostream>
#include <iomanip>

#define ENABLE_DEBUG 1 
#if ENABLE_DEBUG                                                                
#define logger std::cout << "LOG:" << __FILE__ << ":" << __LINE__ << " \t :: \t " 
#else                                                                              
#define debug_out while(0) std::cout                                               
#endif /* ENABLE_DEBUG */     

// Files from flecsi
#include <flecsi/data/data.h>
#include <flecsi/io/io.h>

// Files from flecsi-sp
#include <flecsi-sp/pic/mesh.h>
#include <flecsi-sp/pic/entity_types.h>

// Namespaces
using namespace flecsi;
using namespace flecsi::data;
using namespace flecsi::sp;
using namespace flecsi::sp::pic;

using particle_list_t = particle_list_<float>;
using mesh_t = pic_mesh_t;
using vertex_t = pic_types_t::vertex_t;

using real_t = double;


// Constants
#define AoS // TODO: Need other types
#define NDIM 3 // Number of dimensions, i.e 3D 
// This can be pulled from the PIC class itself 

// Types
using dim_array_t = std::array<real_t,NDIM>;

int num_particles = 0; 

const real_t q = 1; // TODO: Give these values
const real_t m = 1; // TODO: Give these values
const real_t mu = 4.0 * M_PI * 1.0e-7; // permeability of free space
const real_t c = 299792458; // Speed of light   
const real_t eps = 1.0 / (c * c * mu); // permittivity of free space

// TODO: Read input deck
size_t NX_global = 64;
size_t NY_global = 64;
size_t NZ_global = 64;

size_t nx = NX_global;
size_t ny = NY_global;
size_t nz = NZ_global;

size_t NPPC = 32;
double dt = 0.1; // TODO: units?
int num_steps = 10;

real_t len_x_global = 1.0;
real_t len_y_global = 1.0;
real_t len_z_global = 1.0;

real_t len_x = len_x_global;
real_t len_y = len_y_global;
real_t len_z = len_z_global;

real_t dx = len_x/nx;
real_t dy = len_y/ny;
real_t dz = len_z/nz;

// Methods
void init_mesh(mesh_t& m, size_t nx, size_t ny, size_t nz) {

  std::vector<vertex_t *> vs;

  for(size_t k(0); k<nz+1; ++k) {
    for(size_t j(0); j<ny+1; ++j) {
      for(size_t i(0); i<nx+1; ++i) {
        vs.push_back(m.make_vertex({double(i), double(j), double(k)}));
      } // for
    } // for
  } // for

  size_t width = nx+1;
  size_t depth = ny+1;

  for(size_t k(0); k<nz; ++k) { // z
    for(size_t j(0); j<ny; ++j) { // y
      for(size_t i(0); i<nx; ++i) { // x

        bool is_domain_boundary = i==0 || j==0 || i==(nx-1) || j==(ny-1) || 
          k == 0 || k == (nz-1); 

        // Indexing x + y*WIDTH + Z*WIDTH*DEPTH
        m.make_cell({
            vs[ (k    *width*depth) + (j    *width) + i ], // {0, 0, 0} 
            vs[ (k    *width*depth) + (j    *width) + (i+1) ], // {0, 0, 1} 
            vs[ (k    *width*depth) + ((j+1)*width) + (i+1) ], // {0, 1, 1} 
            vs[ (k    *width*depth) + ((j+1)*width) + i ], // {0, 1, 0} 
            vs[ ((k+1)*width*depth) + ((j+1)*width) + i ], // {1, 1, 0} 
            vs[ ((k+1)*width*depth) + (j    *width) + i ], // {1, 0, 0} 
            vs[ ((k+1)*width*depth) + (j    *width) + (i+1) ], // {1, 0, 1} 
            vs[ ((k+1)*width*depth) + ((j+1)*width) + (i+1) ], // {1, 1, 1} 
            }, is_domain_boundary ? cell_type_t::domain_boundary :
            cell_type_t::unknown);
      } // for
    } // for
  } // for

  m.init();
}


dim_array_t cross_product( dim_array_t a, dim_array_t b) {

  dim_array_t result; 

  result[0] = a[1]*b[2] - a[2]*b[1];
  result[1] = a[2]*b[0] - a[0]*b[2];
  result[2] = a[0]*b[1] - a[1]*b[0];

  return result; 

}

void field_initialization(mesh_t& m)
{
  // Experiment with data
  auto jx = get_accessor(m, fields, jx, double, dense, 0);                    
                                                                                     
  // Example of how to iterate vertices from cells
  for ( auto c : m.cells() )
  {
    //std::cout << "Volume " << c->volume() << std::endl;
    for ( auto v : m.vertices(c) ) {
      //std::cout << v->coordinates() << std::endl;                                    
      jx[v] = 1.0;
    }
  }
}


void insert_particle(mesh_t& m, real_t x, real_t y, real_t z)
{
  // TODO: Implement this
  logger << "Insert particles at " << x << ", " << y << "," << z << std::endl;
  num_particles++;
}

real_t random_real(real_t min, real_t max)
{
  float r = (float)rand() / (float)RAND_MAX;
  return min + r * (max - min);
}

void particle_initialization(mesh_t& m) {

  //srand((unsigned)time(0)); 

  for ( auto v : m.vertices() ) {
    auto coord = v->coordinates();

    real_t x_min = coord[0] * dx;
    real_t x_max = x_min + dx;

    real_t y_min = coord[1] * dy;
    real_t y_max = y_min + dy;

    real_t z_min = coord[2] * dz;
    real_t z_max = z_min + dz;
    for (size_t i = 0; i < NPPC; i++)
    {
      real_t x = random_real( x_min, x_max );
      real_t y = random_real( y_min, y_max );
      real_t z = random_real( z_min, z_max );

      insert_particle(m, x, y, z);
    }
  }
  logger << "Done particle init" << std::endl;

}


void init_simulation(mesh_t& m) {
  // TODO: Much of this can be pushed into the specialization 
  
  // Register data
  register_data(m, fields, jx, double, dense, 1, vertices);        
  register_data(m, fields, jy, double, dense, 1, vertices);        
  register_data(m, fields, jz, double, dense, 1, vertices);        

  register_data(m, fields, ex, double, dense, 1, vertices);        
  register_data(m, fields, ey, double, dense, 1, vertices);        
  register_data(m, fields, ez, double, dense, 1, vertices);        

  register_data(m, fields, bx, double, dense, 1, vertices);        
  register_data(m, fields, by, double, dense, 1, vertices);        
  register_data(m, fields, bz, double, dense, 1, vertices);        

  register_data(m, particles, p, particle_list_t, dense, 1, cells);        

  field_initialization(m);
  particle_initialization(m);
}

void field_solve(mesh_t& m, real_t dt) {
  
  // TODO: Double check these are the right values for EM (not ES)

  // General Idea, taken directly from _the_ Yee paper
  //
  // Electric Field 
    //  Take equation for D
    //  Dx_n - Dx_n-1 / dt = (Hz - Hz) / dy - (hy - hy) / dz + Jx 
    //  Rearrange:
    //  Dx_n = ((Hz - Hz) / dy - (hy - hy) / dz + Jx) *dt + Dx_n-1
    //  Swap out D and B:
    //  eps*Ex_n = ( 1/mu * ((Bz - Bz) / dy - (By - By) / dz) + Jx) * dt + eps*Ex_n-1
    //  Alternatively:
    //  Ex_n = (( 1/mu * ((Bz - Bz) / dy - (By - By) / dz) + Jx) * dt)/eps + Ex_n-1
    
  // Magnetic Field 
    // (Bx_h - Bx_-h) / dt  = (Ey - Ey) / dz - (Ez - Ez) / dy
    // Rearrange:
    // Bx = ((Ey - Ey) / dz - (Ez - Ez) / dy) * dt + Bx_-h
  
  // Note: Care needs to be taken as the sign changes from x to y to z 
  
  auto Bx = get_accessor(m, fields, bx, double, dense, 0);                    
  auto By = get_accessor(m, fields, by, double, dense, 0);                    
  auto Bz = get_accessor(m, fields, bz, double, dense, 0);                    

  auto Ex = get_accessor(m, fields, ex, double, dense, 0);                    
  auto Ey = get_accessor(m, fields, ey, double, dense, 0);                    
  auto Ez = get_accessor(m, fields, ez, double, dense, 0);                    

  auto Jx = get_accessor(m, fields, jx, double, dense, 0);                    
  auto Jy = get_accessor(m, fields, jy, double, dense, 0);                    
  auto Jz = get_accessor(m, fields, jz, double, dense, 0);                    

  /* Convert this from 3d loop to flecsi loop
  for (int k = 0; k < NZ; k++) {
    for (int j = 0; j < NY; j++) {
      for (int i = 0; i < NX; i++) {
  */

  for ( auto v : m.vertices() ) {
    /* Convert this from 3d loop to flecsi loop
        Bx[k][j][i] = Bx[k][j][i] + dt * ( (Ey[k+1][j][i] - Ey[k][j][i]) / dz - 
            (Ez[k][j+1][i] - Ez[k][j][i]) / dy );

        By[k][j][i] = By[k][j][i] + dt * ( (Ez[k+1][j][i] - Ez[k][j][i]) / dx - 
            (Ex[k][j+1][i] - Ex[k][j][i]) / dz );

        Bz[k][j][i] = Bz[k][j][i] + dt * ( (Ex[k+1][j][i] - Ex[k][j][i]) / dy - 
            (Ey[k][j+1][i] - Ey[k][j][i]) / dx );

        // TODO: Check these indexs
        Ex[k][j][i] = ( (1/(mu*eps)) * ( ((Bz[k][j][i] - Bz[k][j-1][i] ) / dy ) +
              ((By[k][j][i] - By[k][j][i-1]) / dz)) + Jx[k][j][i]) * dt + Ex[k][j][i];

        Ey[k][j][i] = ( (1/(mu*eps)) * ( ((Bx[k][j][i] - Bx[k][j-1][i] ) / dz ) +
              ((Bz[k][j][i] - Bz[k][j][i-1]) / dx)) + Jy[k][j][i]) * dt + Ey[k][j][i];

        Ez[k][j][i] = ( (1/(mu*eps)) * ( ((By[k][j][i] - By[k][j-1][i] ) / dx ) +
              ((Bx[k][j][i] - Bx[k][j][i-1]) / dy)) + Jz[k][j][i]) * dt + Ez[k][j][i];
    */

    // TODO: Ask Ben how best to deal with stenciling like this
    auto width = ny;
    auto depth = nz;
    auto i = (1);
    auto j = (1*width);
    auto k = (1*width*depth);

    auto v_pk = v+k;
    auto v_pj = v+j;
    auto v_mj = v-j;
    auto v_mi = v-i;

    Bx[v] = Bx[v] + dt * ( (Ey[v_pk] - Ey[v]) / dz - 
        (Ez[v_pj] - Ez[v]) / dy );

    By[v] = By[v] + dt * ( (Ez[v_pk] - Ez[v]) / dx - 
        (Ex[v_pj] - Ex[v]) / dz );

    Bz[v] = Bz[v] + dt * ( (Ex[v_pk] - Ex[v]) / dy - 
        (Ey[v_pj] - Ey[v]) / dx );

    // TODO: Check these indexs
    Ex[v] = ( (1/(mu*eps)) * ( ((Bz[v] - Bz[v_mj] ) / dy ) +
          ((By[v] - By[v_mi]) / dz)) + Jx[v]) * dt + Ex[v];

    Ey[v] = ( (1/(mu*eps)) * ( ((Bx[v] - Bx[v_mj] ) / dz ) +
          ((Bz[v] - Bz[v_mi]) / dx)) + Jy[v]) * dt + Ey[v];

    Ez[v] = ( (1/(mu*eps)) * ( ((By[v] - By[v_mj] ) / dx ) +
          ((Bx[v] - Bx[v_mi]) / dy)) + Jz[v]) * dt + Ez[v];
  }

}
void particle_move() { 

  // mult by delta_t and divide by delta_x 
  // Swap in F/m = qE/m 
  // v = v + (F * delta_t) / m 
  // x = x + v * delta_t

  // Gives:
  // (v*delta_t)/delta_x = (v*delta_t / delta_x) + (q/m)*((E * delta_t^2) / delta_x)

  // KE = m/2 * v_old * v_new 
  //

  /* TODO: Implement this once we have a particle store
     dx += ux*dt;
     dy += uy*dt;
     dz += uz*dt;
     */
}

void update_velocities(mesh_t& mesh, real_t dt) {

  auto Bx = get_accessor(mesh, fields, bx, double, dense, 0);                    
  auto By = get_accessor(mesh, fields, by, double, dense, 0);                    
  auto Bz = get_accessor(mesh, fields, bz, double, dense, 0);                    

  auto Ex = get_accessor(mesh, fields, ex, double, dense, 0);                    
  auto Ey = get_accessor(mesh, fields, ey, double, dense, 0);                    
  auto Ez = get_accessor(mesh, fields, ez, double, dense, 0);                    

  for (size_t i = 0; i < num_particles; i++)
  {

    int cell_index[3];
    cell_index[0] = 1; // TODO: Set this based on particle properties
    cell_index[1] = 1; // TODO: Set this
    cell_index[2] = 1; // TODO: Set this

    dim_array_t E;
    dim_array_t B;

    E[0] = Ex[ cell_index[0] ];
    E[1] = Ey[ cell_index[1] ];
    E[2] = Ez[ cell_index[2] ];

    B[0] = Bx[ cell_index[0] ];
    B[1] = By[ cell_index[1] ];
    B[2] = Bz[ cell_index[2] ];

    // Use the Boris method to update the velocity and rotate (P62 in Birdsall) 
    // Example of this can also be found here 
    // https://www.particleincell.com/wp-content/uploads/2011/07/ParticleIntegrator.java 

    // Equations:

    // v_old = v- - qE/m * delta_t / 2
    // => v- = v + qE/m * delta_t / 2

    // v_new = v+ + qE/m * delta_t / 2

    // v' = v- + v- X t 
    // v+ = v- + v' X s 

    // |v-|^2 = |v+|^2
    // s = 2t / (1+t^2) 
    // t = qB /m * delta_t / 2 
    //
    // TODO: move these data declarations
    dim_array_t t;
    dim_array_t v;
    dim_array_t s;
    dim_array_t v_plus;
    dim_array_t v_minus;
    dim_array_t v_prime;

    // TODO: give E/B a value 
    // TODO: make this do multiple particles..

    real_t t_squared = 0.0;

    // Calculate t and |t^2|
    for (size_t i = 0; i < NDIM; i++) 
    {
      t[i] = q/m * B[i] * 0.5 * dt;
      t_squared += t[i]*t[i];
    }

    // Calculate s
    for (size_t i = 0; i < NDIM; i++) 
    {
      s[i] = 2*t[i] / (1+t_squared);
    }

    // Calculate v-
    for (size_t i = 0; i < NDIM; i++) 
    {
      v_minus[i] = v[i] + q/m * E[i] * 0.5 * dt;
    }

    // Calculate v'
    dim_array_t vt_cross_product = cross_product( v_minus, t);
    for (size_t i = 0; i < NDIM; i++) 
    {
      v_prime[i] = v_minus[i] + vt_cross_product[i];
    }

    // Calculate v+
    dim_array_t vs_cross_product = cross_product( v_prime, s);
    for (size_t i = 0; i < NDIM; i++) 
    {
      v_plus[i] = v_minus[i] + vs_cross_product[i];
    }

    // Calculate v_new
    for (size_t i = 0; i < NDIM; i++) 
    {
      v[i] = v_plus[i] + q/m * E[i] * 0.5 * dt;
    }

    // TODO: Can hoist that q/m E dt/2?

  }
}



void particle_push() {

}

void fields_half() {} 
void fields_final() {} 

// Test main PIC kernel implementation
void kernel() {

  // Equations of motion
  // Leap frog method. 
    // TODO: Does this change in 3d?
    // Integrate force and velocity independently 
    //
    //  m * (dv)/(dt) = F 
    //      (dx)/(dt) = V
    //
    //  Can do this as a finite difference. 
    //
    //  m * (v_new - v_old) / (delta t) = F_old
    //      (x_new - x_old) / (delta t) = v_new
    //
    // Advances v_t to v_(t+delta t) and x_t to x_(t+delta_t)
      // v and x however are at difference times 
     
    // TODO: Push this back
    // Push v back to v_(t-(delta t / 2)) 
     
    // F has two parts 
    // F = F_electric + F_magnetic 
    // F = qE + q(v x B) 
      // Cross product implies a rotation?
      // E and B calculated at particle 
      // Interpolate E and B from grid to the particle
      
    // Field Equations
    
    // Yee Grid
      // Replaces Maxwell's equations with a set of finite difference equations
      
  // FDTD (Finite-Difference Time-Domain)
  
  fields_half();

  particle_push();

  fields_final();

}

void load_default_input_deck()
{
  logger << "Importing Default Input Deck" << std::endl;
}

void read_input_deck() 
{
  // TODO: Empty.. for now...
  // At some point this will read a JSON file (using a lib)
}

void driver(int argc, char ** argv) {

  // Declare mesh
  mesh_t m;

  bool load_input_deck = false;

  if (load_input_deck) {
    read_input_deck();
  }
  else 
  {
    load_default_input_deck();
  }

  // Init mesh
  init_mesh(m, NX_global, NY_global, NZ_global);

  // Init Simulation (generic particles etc)
  init_simulation(m);

  /*
  // Example of how to iterate cells from verticies
  for ( auto v : m.vertices() )
  {
    std::cout << "Volume " << v->coordinates() << std::endl;
    //std::cout << v << std::endl;
    for ( auto c : m.cells(v) ) {
      std::cout << c->volume() << std::endl;                                    
    }
  }
  */

  // Do an initial velocity move of dt/2 backwards to enable the leap frogging
  update_velocities(m, -1 * (dt/2));

  // Perform main loop
  logger << "Starting Main Push" << std::endl;
  for (size_t i = 0; i < num_steps; i++) 
  {
    logger << "-> Start Step " << i << std::endl;
    kernel();
  }
  logger << "Finished Main Push" << std::endl;

  /*
  auto ap = get_mutator(m, solver, p, double, sparse, 0, 3);

  for(auto v: m.vertices()) 
  {
    for(size_t i(0); i<3; ++i) 
    {
      ap(v, i) = 1.0;
    } // for
  } // for

  auto u = get_accessor(m, solver, unknowns, double, dense, 0);

  for(auto v: m.vertices()) {
    std::cout << v->coordinates() << std::endl;
    u[v] = 0.0;
  } // for
  */

#define VIS 0
#if VIS
  // I stole this from flecsale...and need to talk to Marc about how it works 
  
  std::string prefix = "mesh_out";
  std::string postfix = "dat";

  int step = 0;
  std::stringstream ss;
  ss << prefix;
  ss << std::setw( 7 ) << std::setfill( '0' ) << step++;
  ss << "."+postfix;

  flecsi::io::write_mesh( ss.str(), m );
#endif

} // driver

#endif // driver_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
