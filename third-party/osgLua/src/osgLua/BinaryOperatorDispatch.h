/** @file
	@brief Header

	@date 2012

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef INCLUDED_BinaryOperatorDispatch_h_GUID_6c86a34e_7070_4d10_aa19_5ae304064960
#define INCLUDED_BinaryOperatorDispatch_h_GUID_6c86a34e_7070_4d10_aa19_5ae304064960

// Internal Includes
#include "boost/Multiplication.hpp"

// Library/third-party includes
#include <boost/mpl/quote.hpp>
#include <boost/mpl/bind.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/for_each.hpp>

// Standard includes
// - none

namespace osgLua {
	/*
		template<template<typename P1, typename P2> class Op
		*/

	template<typename T>
	inline bool typeUsableAs(const introspection::Type& t) {
		static const introspection::Type& myType =
		    introspection::Reflection::getType(extended_typeid<T>());
		return t == myType || t.isSubclassOf(myType);
	}

	template<typename T>
	inline bool osgLuaValueUsableAs(lua_State * L, int i) {
		Value * a = Value::get(L, i);
		if (a) {
			return typeUsableAs<T>(a->get().getType());
		}
		return false;
	}

	template<typename TypePairOp, template<typename P1, typename P2> class Op>
	class BinaryOperatorApplicationFunctor {
		public:
			BinaryOperatorApplicationFunctor(lua_State * L, introspection::Type const& otherType) : other(otherType), a1(getValue(L, 1)), a2(getValue(L, 2)), r(), success(false) {}

			template<typename T>
			void operator()(boost::mpl::identity<T> const&) {
				if (!success && typeUsableAs<T>(other)) {
					typedef typename boost::mpl::apply<TypePairOp, T>::type Types;
					typedef typename types::first first;
					typedef typename types::second second;
					r = Op<first, second>::apply(introspection::variant_cast<first>(a1), introspection::variant_cast<second>(a2));
					success = true;
				}
			}

			bool pushIfSuccessful(lua_State * L) const {
				if (success) {
					Value::push(L, r);
				}
				return success;
			}

		private:
			introspection::Type const& other;
			introspection::Value a1;
			introspection::Value a2;
			introspection::Value r;
			bool success;
	};

	template<typename T1, template<typename P1, typename P2> class Op>
	struct BinaryOperatorDispatch {
		struct SelfFirst {
			struct Availability {
				template<typename T>
				struct apply {
					typedef osgTraits::is_operation_available<Op<T1, T> > type;
				};
			};

			struct TypePair {
				template<typename T>
				struct apply : boost::mpl::pair<T1, T> {};
			};
		};

		struct SelfSecond {
			struct Availability  {
				template<typename T>
				struct apply {
					typedef osgTraits::is_operation_available<Op<T, T1> > type;
				};
			};

			struct TypePair {
				template<typename T>
				struct apply : boost::mpl::pair<T, T1> {};
			};
		};

		template<typename OrderImpl>
		static bool attemptKnowingOrder(lua_State * L, introspection::Type const& otherType) {
			introspection::Value a1 = getValue(L, 1);
			introspection::Value a2 = getValue(L, 2);
			introspection::Value r;
			typedef boost::mpl::copy_if<osgTraits::math_types, typename OrderImpl::Availability >::type OtherArgumentPossibilities;
			typedef BinaryOperatorApplicationFunctor<typename OrderImpl::TypePair, Op> FunctorType;
			FunctorType f(L, otherType);
			boost::mpl::for_each<OtherArgumentPossibilities, boost::mpl::identity<boost::mpl::_1>,  FunctorType &>(f);
			return f.pushIfSuccessful(L);
		}

		static bool attempt(lua_State * L) {
			if (osgLuaValueUsableAs<T1>(L, 1)) {
				return attemptKnowingOrder<SelfFirst>(L, getValue(L, 2).getType());
			} else if (osgLuaValueUsableAs<T1>(L, 2)) {
				return attemptKnowingOrder<SelfSecond>(L, getValue(L, 1).getType());
			}
			return false;
		}
	};
} // end of namespace osgLua

#endif // INCLUDED_BinaryOperatorDispatch_h_GUID_6c86a34e_7070_4d10_aa19_5ae304064960