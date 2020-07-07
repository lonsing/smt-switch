/*********************                                                        */
/*! \file btor-simple.cpp
** \verbatim
** Top contributors (to current version):
**   Makai Mann
** This file is part of the smt-switch project.
** Copyright (c) 2020 by the authors listed in the file AUTHORS
** in the top-level source directory) and their institutional affiliations.
** All rights reserved.  See the file LICENSE in the top-level source
** directory for licensing information.\endverbatim
**
** \brief
**
**
**/

#include <iostream>
#include <memory>
#include <vector>
#include "assert.h"

#include "boolector_factory.h"
#include "smt.h"
// after a full installation
// #include "smt-switch/boolector_factory.h"
// #include "smt-switch/smt.h"

using namespace smt;
using namespace std;

int main()
{
  SmtSolver s = BoolectorSolverFactory::create(false);
  s->set_logic("QF_ABV");
  s->set_opt("produce-models", "true");
  Sort bvsort8 = s->make_sort(BV, 8);
  Term x = s->make_symbol("x", bvsort8);
  Term y = s->make_symbol("y", bvsort8);
  Term xpy = s->make_term(BVAdd, x, y);

  TermVec vec;
  UnorderedTermSet set;
  uint32_t i, found = 0, max = 4000000;

  set.max_load_factor(10);
  
  //for (auto c : xpy)
  for (i = 1; i <= max; i++)
  {
    string name = "x" + to_string(i);
    //cout << "TEST: " << name << "\n";
    Term newterm = s->make_symbol(name, bvsort8);
    Term t = s->make_term(BVAdd, x, newterm);
    //cout << "Hash: " << (t->hash()) << " for term "<< t.get() << "\n";
    vec.push_back(t);
    set.insert(t);
    if ((i & (1024 - 1)) == 0)
      cout << "i: " << i << " buckets: " << set.bucket_count() << " load factor: " << set.load_factor() << " max load factor "<< set.max_load_factor() << "\n";
  }

  //code adapted from stackoverflow
  size_t collisions = 0, max_bucket_size = 0, empty = 0;
  for (auto bucket = set.bucket_count(); bucket--;) {
    if (set.bucket_size(bucket) == 0)
      empty++;
    else
      {
        if (set.bucket_size(bucket) > max_bucket_size)
          max_bucket_size = set.bucket_size(bucket);
        collisions += set.bucket_size(bucket) - 1;
      }
  }
  std::cout << " DONE set = " << set.max_load_factor() << ' ' << set.load_factor() << ' '
            << ' ' << set.bucket_count() << ' ' << collisions << ' ' << empty << ' ' <<  max_bucket_size <<'\n';

  
  for (i = 1; i <= max; i++)
    {
      Term t = vec[i-1];
      if (set.find (t) != set.end())
          found++;
    }

  cout << "found: " << found << "\n";

  return 0;
}
