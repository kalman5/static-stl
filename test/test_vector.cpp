/*
Copyright © 2015 Kean Mariotti <kean.mariotti@gmail.com>
This work is free. You can redistribute it and/or modify it under the
terms of the Do What The Fuck You Want To Public License, Version 2,
as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.
*/

#include <catch.hpp>
#include <sstl/vector.h>

#include "utility.h"
#include "counted_type.h"

namespace sstl
{
namespace test
{

using vector_int_t = vector<int, 11>;
using vector_counted_type_t = vector<counted_type, 11>;

TEST_CASE("vector")
{
   SECTION("default constructor")
   {
      auto v = vector_int_t();
      REQUIRE(v.empty());
   }

   SECTION("count constructor")
   {
      SECTION("default value")
      {
         auto expected = {0, 0, 0, 0, 0};
         auto v = vector_int_t(5);
         REQUIRE(are_containers_equal(v, expected));
      }
      SECTION("custom value")
      {
         auto expected = {3, 3, 3, 3, 3};
         auto v = vector_int_t(5, 3);
         REQUIRE(are_containers_equal(v, expected));
      }
   }

   SECTION("copy constructor")
   {
      SECTION("contained values")
      {
         auto rhs = vector_int_t{1, 2, 3};
         auto lhs = vector_int_t{ rhs };
         auto expected = {1, 2, 3};
         REQUIRE(are_containers_equal(lhs, expected));
      }
      SECTION("number of copy constructions")
      {
         auto rhs = vector_counted_type_t{1, 2, 3};
         counted_type::reset_counts();
         auto lhs = vector_counted_type_t{ rhs };
         REQUIRE(counted_type::check().copy_constructions(3));
      }
   }

   SECTION("move constructor")
   {
      SECTION("contained values")
      {
         auto rhs = vector_int_t{1, 2, 3};
         auto lhs = vector_int_t{ rhs };
         auto expected = {1, 2, 3};
         REQUIRE(are_containers_equal(lhs, expected));
      }
      SECTION("number of move constructions")
      {
         auto rhs = vector_counted_type_t{1, 2, 3};
         counted_type::reset_counts();
         auto lhs = vector_counted_type_t{ std::move(rhs) };
         REQUIRE(counted_type::check().move_constructions(3));
      }
   }

   SECTION("range constructor")
   {
      auto range = {1, 2, 3};
      auto v = vector_int_t(range.begin(), range.end());
      REQUIRE(are_containers_equal(v, range));
   }

   SECTION("initializer-list constructor")
   {
      auto init = std::initializer_list<int>{1, 2, 3};
      auto v = vector_int_t(init);
      REQUIRE(are_containers_equal(v, init));
   }

   SECTION("destructor (contained values are destroyed)")
   {
      {
         auto v = vector_counted_type_t(7);
         counted_type::reset_counts();
      }
      REQUIRE(counted_type::check().destructions(7));
   }

   SECTION("copy assignment operator")
   {
      SECTION("contained values")
      {
         auto rhs = vector_int_t{1, 2, 3};
         auto lhs = vector_int_t{};
         lhs = rhs;
         REQUIRE(are_containers_equal(lhs, rhs));
      }
      SECTION("number of copy assignments")
      {
         auto rhs = vector_counted_type_t{1, 2, 3, 4, 5};
         auto lhs = vector_counted_type_t{1, 2, 3};
         counted_type::reset_counts();
         lhs = rhs;
         REQUIRE(counted_type::check().copy_assignments(3));
      }
      SECTION("number of copy constructions")
      {
         auto rhs = vector_counted_type_t{1, 2, 3, 4, 5};
         auto lhs = vector_counted_type_t{1, 2, 3};
         counted_type::reset_counts();
         lhs = rhs;
         REQUIRE(counted_type::check().copy_constructions(2));
      }
      SECTION("number of destructions")
      {
         auto rhs = vector_counted_type_t{1, 2, 3};
         auto lhs = vector_counted_type_t{1, 2, 3, 4, 5};
         counted_type::reset_counts();
         lhs = rhs;
         REQUIRE(counted_type::check().destructions(2));
      }
   }

   SECTION("move assignment operator")
   {
      SECTION("contained values")
      {
         auto rhs = vector_int_t{1, 2, 3};
         auto lhs = vector_int_t{};
         lhs = std::move(rhs);
         REQUIRE(are_containers_equal(lhs, rhs));
      }
      SECTION("number of move assignments")
      {
         auto rhs = vector_counted_type_t{1, 2, 3, 4, 5};
         auto lhs = vector_counted_type_t{1, 2, 3};
         counted_type::reset_counts();
         lhs = std::move(rhs);
         REQUIRE(counted_type::check().move_assignments(3));
      }
      SECTION("number of copy constructions")
      {
         auto rhs = vector_counted_type_t{1, 2, 3, 4, 5};
         auto lhs = vector_counted_type_t{1, 2, 3};
         counted_type::reset_counts();
         lhs = std::move(rhs);
         REQUIRE(counted_type::check().move_constructions(2));
      }
      SECTION("number of destructions")
      {
         auto rhs = vector_counted_type_t{1, 2, 3};
         auto lhs = vector_counted_type_t{1, 2, 3, 4, 5};
         counted_type::reset_counts();
         lhs = std::move(rhs);
         REQUIRE(counted_type::check().destructions(2));
      }
   }

   SECTION("initializer-list assignment operator")
   {
      SECTION("contained values")
      {
         auto rhs = {1, 2, 3};
         auto lhs = vector_int_t{};
         lhs = rhs;
         REQUIRE(are_containers_equal(lhs, rhs));
      }
      SECTION("number of copy assignments")
      {
         auto rhs = std::initializer_list<counted_type>{1, 2, 3, 4, 5};
         auto lhs = vector_counted_type_t{1, 2, 3};
         counted_type::reset_counts();
         lhs = rhs;
         REQUIRE(counted_type::check().copy_assignments(3));
      }
      SECTION("number of copy constructions")
      {
         auto rhs = std::initializer_list<counted_type>{1, 2, 3, 4, 5};
         auto lhs = vector_counted_type_t{1, 2, 3};
         counted_type::reset_counts();
         lhs = rhs;
         REQUIRE(counted_type::check().copy_constructions(2));
      }
      SECTION("number of destructions")
      {
         auto rhs = std::initializer_list<counted_type>{1, 2, 3};
         auto lhs = vector_counted_type_t{1, 2, 3, 4, 5};
         counted_type::reset_counts();
         lhs = rhs;
         REQUIRE(counted_type::check().destructions(2));
      }
   }

   SECTION("count assign")
   {
      SECTION("contained values")
      {
         auto v = vector_int_t{1, 2, 3};
         v.assign(5, 7);
         auto expected = {7, 7, 7, 7, 7};
         REQUIRE(are_containers_equal(v, expected));
      }
      SECTION("number of copy assignments / copy constructions")
      {
         auto v = vector_counted_type_t{1, 2, 3};
         counted_type::reset_counts();
         v.assign(5, 7);
         REQUIRE(counted_type::check().copy_assignments(3).copy_constructions(2));
      }
      SECTION("number of destructions")
      {
         auto v = vector_counted_type_t{1, 2, 3, 4, 5};
         auto value = counted_type{ 7 };
         counted_type::reset_counts();
         v.assign(3, value);
         REQUIRE(counted_type::check().destructions(2));
      }
   }

   SECTION("range assign")
   {
      SECTION("contained values")
      {
         auto range = {7, 7, 7, 7, 7};
         auto v = vector_int_t{1, 2, 3};
         v.assign(range.begin(), range.end());
         REQUIRE(are_containers_equal(v, range));
      }
      SECTION("number of copy assignments / copy constructions")
      {
         auto range = std::initializer_list<counted_type>{7, 7, 7, 7, 7};
         auto v = vector_counted_type_t{1, 2, 3};
         counted_type::reset_counts();
         v.assign(range.begin(), range.end());
         REQUIRE(counted_type::check().copy_assignments(3).copy_constructions(2));
      }
      SECTION("number of destructions")
      {
         auto range = std::initializer_list<counted_type>{7, 7, 7};
         auto v = vector_counted_type_t{1, 2, 3, 4, 5};
         counted_type::reset_counts();
         v.assign(range.begin(), range.end());
         REQUIRE(counted_type::destructions == 2);
         REQUIRE(counted_type::check().destructions(2));
      }
   }

   SECTION("at")
   {
      auto v = vector_int_t{1, 2, 3, 4, 5};
      SECTION("read access")
      {
         const auto& cv = v;
         REQUIRE(v.at(0) == 1);
         REQUIRE(v.at(2) == 3);
         REQUIRE(v.at(4) == 5);
      }
      SECTION("write access")
      {
         v.at(0) = 10;
         v.at(2) = 30;
         v.at(4) = 50;
         REQUIRE(v.at(0) == 10);
         REQUIRE(v.at(2) == 30);
         REQUIRE(v.at(4) == 50);
      }
   }

   SECTION("operator[]")
   {
      auto v = vector_int_t{1, 2, 3, 4, 5};
      SECTION("read access")
      {
         const auto& cv = v;
         REQUIRE(v[0] == 1);
         REQUIRE(v[2] == 3);
         REQUIRE(v[4] == 5);
      }
      SECTION("write access")
      {
         v[0] = 10;
         v[2] = 30;
         v[4] = 50;
         REQUIRE(v[0] == 10);
         REQUIRE(v[2] == 30);
         REQUIRE(v[4] == 50);
      }
   }

   SECTION("front")
   {
      auto v = vector_int_t{1, 2, 3};
      SECTION("read access")
      {
         const auto& cv = v;
         REQUIRE(v.front() == 1);
      }
      SECTION("write access")
      {
         v.front() = 10;
         REQUIRE(v.front() == 10);
      }
   }

   SECTION("data")
   {
      auto v = vector_int_t{1, 2, 3};
      SECTION("read access")
      {
         const auto& cv = v;
         REQUIRE(*v.data() == 1);
      }
      SECTION("write access")
      {
         *v.data() = 10;
         REQUIRE(v[0] == 10);
      }
   }

   SECTION("iterators")
   {
      SECTION("zero elements")
      {
         auto v = vector_int_t{};
         const auto& cv = v;

         REQUIRE(v.begin() == v.end());
         REQUIRE(cv.cbegin() == cv.cend());
         REQUIRE(v.rbegin() == v.rend());
         REQUIRE(cv.crbegin() == cv.crend());
      }
      SECTION("one elements")
      {
         auto v = vector_int_t{1};
         const auto& cv = v;

         REQUIRE(std::distance(v.begin(), v.end()) == 1);
         REQUIRE(std::distance(cv.cbegin(), cv.cend()) == 1);
         REQUIRE(std::distance(v.rbegin(), v.rend()) == 1);
         REQUIRE(std::distance(cv.crbegin(), cv.crend()) == 1);

         REQUIRE(*v.begin() == 1);
         REQUIRE(*cv.cbegin() == 1);
         REQUIRE(*v.rbegin() == 1);
         REQUIRE(*cv.crbegin() == 1);
      }
      SECTION("many elements")
      {
         auto l = std::initializer_list<int>{1, 2, 3, 4, 5};
         auto lbegin = l.begin();
         auto lend = l.end();
         auto lrbegin = std::reverse_iterator<decltype(lend)>{ lend };
         auto lrend = std::reverse_iterator<decltype(lbegin)>{ lbegin };

         auto v = vector_int_t(l);
         const auto& cv = v;

         REQUIRE(std::distance(v.begin(), v.end()) == 5);
         REQUIRE(std::distance(cv.cbegin(), cv.cend()) == 5);
         REQUIRE(std::distance(v.rbegin(), v.rend()) == 5);
         REQUIRE(std::distance(cv.crbegin(), cv.crend()) == 5);

         REQUIRE(std::equal(lbegin, lend, v.begin()));
         REQUIRE(std::equal(lbegin, lend, cv.cbegin()));
         REQUIRE(std::equal(lrbegin, lrend, v.rbegin()));
         REQUIRE(std::equal(lrbegin, lrend, cv.crbegin()));
      }
   }

   SECTION("empty / size")
   {
      auto v = vector_int_t{};
      SECTION("zero elements")
      {
         REQUIRE(v.empty());
         REQUIRE(v.size() == 0);
      }
      SECTION("one elements")
      {
         v.push_back(0);
         REQUIRE(!v.empty());
         REQUIRE(v.size() == 1);

         SECTION("many elements")
         {
            v.push_back(0); v.push_back(0);
            REQUIRE(!v.empty());
            REQUIRE(v.size() == 3);

            SECTION("zero elements (again)")
            {
               v.pop_back(); v.pop_back(); v.pop_back();
               REQUIRE(v.empty());
               REQUIRE(v.size() == 0);
            }
         }
      }
   }

   SECTION("max size / capacity")
   {
      SECTION("capacity is 1")
      {
         auto v = sstl::vector<int, 1>{};
         REQUIRE(v.max_size() == 1);
         REQUIRE(v.capacity() == 1);
      }
      SECTION("capacity is 11")
      {
         auto v = sstl::vector<int, 11>{};
         REQUIRE(v.max_size() == 11);
         REQUIRE(v.capacity() == 11);
      }
   }

   SECTION("clear")
   {
      SECTION("contained values")
      {
         auto v = vector_int_t{1, 2, 3, 4, 5, 6, 7};
         v.clear();
         REQUIRE(v.empty());
         v.assign(7, 0);
         REQUIRE(!v.empty());
         v.clear();
         REQUIRE(v.empty());
      }
      SECTION("number of destructions")
      {
         auto v = vector_counted_type_t(7);
         counted_type::reset_counts();
         v.clear();
         REQUIRE(counted_type::check().destructions(7));
         v.assign(7, 0);
         counted_type::reset_counts();
         v.clear();
         REQUIRE(counted_type::check().destructions(7));
      }
   }

   SECTION("lvalue insert")
   {
      SECTION("begin")
      {
         SECTION("contained values")
         {
            auto expected = {7, 3, 3, 3, 3, 3};
            auto v = vector_int_t{3, 3, 3, 3, 3};
            auto pos = v.insert(v.begin(), 7);
            REQUIRE(*pos == 7);
            REQUIRE(are_containers_equal(v, expected));
         }
         SECTION("number of operations")
         {
            auto value = counted_type{};
            auto v = vector_counted_type_t(5);
            counted_type::reset_counts();
            v.insert(v.begin(), value);
            REQUIRE(counted_type::check().move_constructions(1).move_assignments(4).copy_assignments(1));
         }
      }
      SECTION("end")
      {
         SECTION("contained values")
         {
            auto expected = {3, 3, 3, 3, 3, 7};
            auto v = vector_int_t{3, 3, 3, 3, 3};
            auto pos = v.insert(v.end(), 7);
            REQUIRE(*pos == 7);
            REQUIRE(are_containers_equal(v, expected));
         }
         SECTION("number of operations")
         {
            auto value = counted_type{};
            auto v = vector_counted_type_t(5);
            counted_type::reset_counts();
            v.insert(v.end(), value);
            REQUIRE(counted_type::check().copy_constructions(1));
         }
      }
      SECTION("middle")
      {
         SECTION("contained values")
         {
            auto expected = {3, 3, 7, 3, 3, 3};
            auto v = vector_int_t{3, 3, 3, 3, 3};
            auto pos = v.insert(v.begin() + 2, 7);
            REQUIRE(*pos == 7);
            REQUIRE(are_containers_equal(v, expected));
         }
         SECTION("number of operations")
         {
            auto value = counted_type{};
            auto v = vector_counted_type_t(5);
            counted_type::reset_counts();
            v.insert(v.begin()+2, value);
            REQUIRE(counted_type::check().move_constructions(1).move_assignments(2).copy_assignments(1));
         }
      }
   }
}

}
}
