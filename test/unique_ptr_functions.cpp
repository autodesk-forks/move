//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2014-2014.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/move for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#include <boost/move/make_unique.hpp>
#include <boost/core/lightweight_test.hpp>

struct A
{
   int a, b, c;
   static int count;
   A() : a (999), b(1000), c(1001) {++count;}
   A(int a) : a (a), b(1000), c(1001) {++count;}
   A(int a, int b) : a (a), b(b), c(1001) {++count;}
   A(int a, int b, int c) : a (a), b(b), c(c) {++count;}
   A(const A&) {++count;}
   virtual ~A() {--count;}
};

int A::count = 0;

struct B
   : public A
{
   static int count;
   B() {++count;}
   B(const B&) {++count;}
   virtual ~B() {--count;}
};

int B::count = 0;

void reset_counters()
{  A::count = B::count = 0;  }

namespace bml = ::boost::movelib;

////////////////////////////////
//   make_unique_single
////////////////////////////////

namespace make_unique_single{

void test()
{
   //Single element deleter
   reset_counters();
   {
   bml::unique_ptr<A> p(bml::make_unique<A>());
   BOOST_TEST(A::count == 1);
   BOOST_TEST(p->a == 999);
   BOOST_TEST(p->b == 1000);
   BOOST_TEST(p->c == 1001);
   }
   {
   bml::unique_ptr<A> p(bml::make_unique<A>(0));
   BOOST_TEST(A::count == 1);
   BOOST_TEST(p->a == 0);
   BOOST_TEST(p->b == 1000);
   BOOST_TEST(p->c == 1001);
   }
   BOOST_TEST(A::count == 0);
   {
   bml::unique_ptr<A> p(bml::make_unique<A>(0, 1));
   BOOST_TEST(A::count == 1);
   BOOST_TEST(p->a == 0);
   BOOST_TEST(p->b == 1);
   BOOST_TEST(p->c == 1001);
   }
   BOOST_TEST(A::count == 0);
   {
   bml::unique_ptr<A> p(bml::make_unique<A>(0, 1, 2));
   BOOST_TEST(A::count == 1);
   BOOST_TEST(p->a == 0);
   BOOST_TEST(p->b == 1);
   BOOST_TEST(p->c == 2);
   }
   BOOST_TEST(A::count == 0);
}

}  //namespace make_unique_single{

////////////////////////////////
//   make_unique_single
////////////////////////////////

namespace make_unique_array{

void test()
{
   //Single element deleter
   reset_counters();
   {
      bml::unique_ptr<A[]> p(bml::make_unique<A[]>(10));
      BOOST_TEST(A::count == 10);
      for(int i = 0; i != 10; ++i){
         BOOST_TEST(p[i].a == 999);
         BOOST_TEST(p[i].b == 1000);
         BOOST_TEST(p[i].c == 1001);
      }
   }
   BOOST_TEST(A::count == 0);
}

}  //namespace make_unique_array{

////////////////////////////////
//       unique_compare
////////////////////////////////

namespace unique_compare{

void test()
{
   //Single element deleter
   reset_counters();
   {
      bml::unique_ptr<A> pa(bml::make_unique<A>());
      bml::unique_ptr<A> pb(bml::make_unique<A>());
      BOOST_TEST(A::count == 2);

      //Take references to less and greater
      bml::unique_ptr<A> &rpl = pa < pb ? pa : pb;
      bml::unique_ptr<A> &rpg = pa < pb ? pb : pa;

      //Now test operations with .get()

      //Equal
      BOOST_TEST(rpl == rpl && rpl.get() == rpl.get());
      BOOST_TEST(!(rpl == rpg) && !(rpl.get() == rpg.get()));
      //Unequal
      BOOST_TEST(rpl != rpg && rpl.get() != rpg.get());
      BOOST_TEST(!(rpl != rpl) && !(rpl.get() != rpl.get()));
      //Less
      BOOST_TEST(rpl < rpg && rpl.get() < rpg.get());
      BOOST_TEST(!(rpg < rpl) && !(rpg.get() < rpl.get()));
      //Greater
      BOOST_TEST(rpg > rpl && rpg.get() > rpl.get());
      BOOST_TEST(!(rpg > rpg) && !(rpg.get() > rpg.get()));
      //Less or equal
      BOOST_TEST(rpl <= rpg && rpl.get() <= rpg.get());
      BOOST_TEST(rpl <= rpl && rpl.get() <= rpl.get());
      BOOST_TEST(!(rpg <= rpl) && !(rpg.get() <= rpl.get()));
      //Greater or equal
      BOOST_TEST(rpg >= rpl && rpg.get() >= rpl.get());
      BOOST_TEST(rpg >= rpg && rpg.get() >= rpg.get());
      BOOST_TEST(!(rpl >= rpg) && !(rpl.get() >= rpg.get()));
   }
   BOOST_TEST(A::count == 0);
}

}  //namespace unique_compare{


////////////////////////////////
//       unique_compare_zero
////////////////////////////////
namespace unique_compare_zero{

void test()
{
   //Single element deleter
   reset_counters();
   {
      bml::unique_ptr<A> pa(bml::make_unique<A>());
      bml::unique_ptr<A> pb;
      BOOST_TEST(A::count == 1);

      //Equal
      BOOST_TEST(!(pa == 0));
      BOOST_TEST(!(0  == pa));
      BOOST_TEST((pb == 0));
      BOOST_TEST((0  == pb));
      //Unequal
      BOOST_TEST((pa != 0));
      BOOST_TEST((0  != pa));
      BOOST_TEST(!(pb != 0));
      BOOST_TEST(!(0  != pb));
      //Less
      BOOST_TEST((pa < 0) == (pa.get() < 0));
      BOOST_TEST((0 < pa) == (0 < pa.get()));
      BOOST_TEST((pb < 0) == (pb.get() < 0));
      BOOST_TEST((0 < pb) == (0 < pb.get()));
      //Greater
      BOOST_TEST((pa > 0) == (pa.get() > 0));
      BOOST_TEST((0 > pa) == (0 > pa.get()));
      BOOST_TEST((pb > 0) == (pb.get() > 0));
      BOOST_TEST((0 > pb) == (0 > pb.get()));
      //Less or equal
      BOOST_TEST((pa <= 0) == (pa.get() <= 0));
      BOOST_TEST((0 <= pa) == (0 <= pa.get()));
      BOOST_TEST((pb <= 0) == (pb.get() <= 0));
      BOOST_TEST((0 <= pb) == (0 <= pb.get()));
      //Greater or equal
      BOOST_TEST((pa >= 0) == (pa.get() >= 0));
      BOOST_TEST((0 >= pa) == (0 >= pa.get()));
      BOOST_TEST((pb >= 0) == (pb.get() >= 0));
      BOOST_TEST((0 >= pb) == (0 >= pb.get()));
   }
   BOOST_TEST(A::count == 0);
}

}  //namespace unique_compare_zero{

////////////////////////////////
//       unique_compare_nullptr
////////////////////////////////

namespace unique_compare_nullptr{

void test()
{
   #if !defined(BOOST_NO_CXX11_NULLPTR)
   //Single element deleter
   reset_counters();
   {
      bml::unique_ptr<A> pa(bml::make_unique<A>());
      bml::unique_ptr<A> pb;
      BOOST_TEST(A::count == 1);

      //Equal
      BOOST_TEST(!(pa == nullptr));
      BOOST_TEST(!(nullptr  == pa));
      BOOST_TEST((pb == nullptr));
      BOOST_TEST((nullptr  == pb));
      //Unequal
      BOOST_TEST((pa != nullptr));
      BOOST_TEST((nullptr  != pa));
      BOOST_TEST(!(pb != nullptr));
      BOOST_TEST(!(nullptr  != pb));
      //Less
      BOOST_TEST((pa < nullptr) == (pa.get() < nullptr));
      BOOST_TEST((nullptr < pa) == (nullptr < pa.get()));
      BOOST_TEST((pb < nullptr) == (pb.get() < nullptr));
      BOOST_TEST((nullptr < pb) == (nullptr < pb.get()));
      //Greater
      BOOST_TEST((pa > nullptr) == (pa.get() > nullptr));
      BOOST_TEST((nullptr > pa) == (nullptr > pa.get()));
      BOOST_TEST((pb > nullptr) == (pb.get() > nullptr));
      BOOST_TEST((nullptr > pb) == (nullptr > pb.get()));
      //Less or equal
      BOOST_TEST((pa <= nullptr) == (pa.get() <= nullptr));
      BOOST_TEST((nullptr <= pa) == (nullptr <= pa.get()));
      BOOST_TEST((pb <= nullptr) == (pb.get() <= nullptr));
      BOOST_TEST((nullptr <= pb) == (nullptr <= pb.get()));
      //Greater or equal
      BOOST_TEST((pa >= nullptr) == (pa.get() >= nullptr));
      BOOST_TEST((nullptr >= pa) == (nullptr >= pa.get()));
      BOOST_TEST((pb >= nullptr) == (pb.get() >= nullptr));
      BOOST_TEST((nullptr >= pb) == (nullptr >= pb.get()));
   }
   BOOST_TEST(A::count == 0);
   #endif   //#if !defined(BOOST_NO_CXX11_NULLPTR)
}

}  //namespace unique_compare_nullptr{


////////////////////////////////
//             main
////////////////////////////////
int main()
{
   make_unique_single::test();
   make_unique_array::test();
   unique_compare::test();
   unique_compare_zero::test();
   unique_compare_nullptr::test();

   //Test results
   return boost::report_errors();
}