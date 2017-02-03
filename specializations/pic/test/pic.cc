/*~-------------------------------------------------------------------------~~*
 * Copyright (c) 2014 Los Alamos National Security, LLC
 * All rights reserved.
 *~-------------------------------------------------------------------------~~*/

#include "flecsi-sp/pic/mesh.h"

using namespace flecsi::sp;
using vertex_t = pic_mesh_t::vertex_t;

class pic_t
  : public ::testing::Test
{
protected:

  static constexpr size_t N = 2;
  pic_mesh_t m;

  void
  SetUp()
  {
    std::vector<vertex_t *> vs;

    for(size_t j(0); j<N+1; ++j) {
      for(size_t i(0); i<N+1; ++i) {
        vs.push_back(m.make_vertex({double(i), double(j)}));
      } // for
    } // for

    size_t width = N+1;

    for(size_t j(0); j<N; ++j) {
      for(size_t i(0); i<N; ++i) {
        bool is_domain_boundary = i==0 || j==0 || i==(N-1) || j==(N-1);
        m.make_cell({
          vs[ i    + ( j    * width)],
          vs[(i+1) + ( j    * width)],
          vs[(i+1) + ((j+1) * width)],
          vs[ i    + ((j+1) * width)]
        }, is_domain_boundary ? cell_type_t::domain_boundary :
          cell_type_t::unknown);
      } // for
    } // for

    m.init();

  } // SetUp

  virtual void TearDown() {}

}; // class pic_t

TEST_F(pic_t, sanity) {

  register_data( 


  // TODO: This is where I'm going to draft out the algorithm  I want to use.
    //  Hopefully doing it this way will help expose what I need access to

  //////// Plan ////////
  //
  // Define a grid. 
  //
  //
  ///// METHOD /////
  // Leap frog mehtod. Define velocity and position at different times, and do:
  //
  // 1D (?) Finite difference
  // F_old = m( (v_new - v_old) / delta t ) 
  // V_new = (x_new - x_old) / delta t 
  //
  // But they're at different times. Need to push v back to -1/2(delta t) 
  // using F at t=0. Also need to update kinetic and potential/field energies 
  //
  // Field Equations:
  // F = F_e + F_B 
  // F = qE + q(v X B)  (cross product?)
  // E and B are done at the particle, and must be interpolated.


} // TEST

/*----------------------------------------------------------------------------*
 * Cinch test Macros
 *
 *  ==== I/O ====
 *  CINCH_CAPTURE()              : Insertion stream for capturing output.
 *                                 Captured output can be written or
 *                                 compared using the macros below.
 *
 *    EXAMPLE:
 *      CINCH_CAPTURE() << "My value equals: " << myvalue << std::endl;
 *
 *  CINCH_COMPARE_BLESSED(file); : Compare captured output with
 *                                 contents of a blessed file.
 *
 *  CINCH_WRITE(file);           : Write captured output to file. 
 *
 *  CINCH_ASSERT(ASSERTION, ...) : Call Google test macro and automatically
 *                                 dump captured output (from CINCH_CAPTURE)
 *                                 on failure.
 *
 *  CINCH_EXPECT(ASSERTION, ...) : Call Google test macro and automatically
 *                                 dump captured output (from CINCH_CAPTURE)
 *                                 on failure.
 *
 * Google Test Macros
 *
 * Basic Assertions:
 *
 *  ==== Fatal ====             ==== Non-Fatal ====
 *  ASSERT_TRUE(condition);     EXPECT_TRUE(condition)
 *  ASSERT_FALSE(condition);    EXPECT_FALSE(condition)
 *
 * Binary Comparison:
 *
 *  ==== Fatal ====             ==== Non-Fatal ====
 *  ASSERT_EQ(val1, val2);      EXPECT_EQ(val1, val2)
 *  ASSERT_NE(val1, val2);      EXPECT_NE(val1, val2)
 *  ASSERT_LT(val1, val2);      EXPECT_LT(val1, val2)
 *  ASSERT_LE(val1, val2);      EXPECT_LE(val1, val2)
 *  ASSERT_GT(val1, val2);      EXPECT_GT(val1, val2)
 *  ASSERT_GE(val1, val2);      EXPECT_GE(val1, val2)
 *
 * String Comparison:
 *
 *  ==== Fatal ====                     ==== Non-Fatal ====
 *  ASSERT_STREQ(expected, actual);     EXPECT_STREQ(expected, actual)
 *  ASSERT_STRNE(expected, actual);     EXPECT_STRNE(expected, actual)
 *  ASSERT_STRCASEEQ(expected, actual); EXPECT_STRCASEEQ(expected, actual)
 *  ASSERT_STRCASENE(expected, actual); EXPECT_STRCASENE(expected, actual)
 *----------------------------------------------------------------------------*/

/*~------------------------------------------------------------------------~--*
 * Formatting options for vim.
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~------------------------------------------------------------------------~--*/
