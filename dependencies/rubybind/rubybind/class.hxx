// Copyright (c)  2008 Borislav Stanimirov

// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:

// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

#pragma once

// #include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <cassert>

#include "types.hxx"
#include "globals.hxx"
#include "value_cast.hxx"


namespace rubybind
{
	namespace detail
	{
		class empty	{};

		template <class RealClass>
		class class_base //: public ::boost::noncopyable
		{
		protected:
			static VALUE rubyClass_;
		};
/*
		template <class RealClass>
		VALUE class_base<RealClass>::rubyClass_;
 */
		template <>
		class class_base<empty>
		{
		protected:
			static const VALUE& rubyClass_;
		};
	}

	template< typename RealClass, typename Parent = detail::empty, typename Allocator = ::std::allocator<unsigned char> >
	class class_ : public detail::class_base<RealClass>
	{
	public:
		class_(const char* rname)
		{
			construct(rname, rb_cObject);
		}
	protected:
		friend class module;
		//to be called from modules
		class_(const char* rname, VALUE module)
		{
			construct(rname, module);
		}

		typedef detail::class_base<RealClass> BaseClass;
		typedef detail::class_base<Parent> ParentType;
	public:
		//////////////////////////////////////////////////////////////////////
		// constructors

		class_& ctor0()
		{
			rb_define_method(BaseClass::rubyClass_, "initialize0", vf_cast(wrap_ctor0), -1);
			return *this;
		}

		template <typename P1>
		class_& ctor1()
		{
			//note the use &class_::wrap_ctor1<int> instead of just wrap_ctor1<int>
			//this seems to be a vc2003 bug
			rb_define_method(BaseClass::rubyClass_, "initialize1", vf_cast(&class_::wrap_ctor1<int>), -1);
			return *this;
		}

		//////////////////////////////////////////////////////////////////////
		// defs

#		define DEF0 \
		typedef method_wrap0<Unique, ReturnType> method_wrap; \
		method_wrap::func = method; \
		rb_define_method(BaseClass::rubyClass_, rname, vf_cast(method_wrap::wrap_method), -1); \
		return *this;

		template <int Unique, typename ReturnType>
		class_& real_def(const char* rname, ReturnType (RealClass::*method)())
		{
			DEF0
		}

		template <int Unique, typename ReturnType>
		class_& real_def(const char* rname, ReturnType (RealClass::*method)() const)
		{
			DEF0
		}

#		undef DEF0
#		define DEF1 \
		typedef method_wrap1<Unique, ReturnType, P1> method_wrap; \
		method_wrap::func = method; \
		rb_define_method(BaseClass::rubyClass_, rname, vf_cast(method_wrap::wrap_method), -1); \
		return *this;

		template <int Unique, typename ReturnType, typename P1>
		class_& real_def(const char* rname, ReturnType (RealClass::*method)(P1 p1))
		{
			DEF1
		}

		template <int Unique, typename ReturnType, typename P1>
		class_& real_def(const char* rname, ReturnType (RealClass::*method)(P1 p1) const)
		{
			DEF1
		}

#		undef DEF1

		//////////////////////////////////////////////////////////////////////
		// method wraps

		template <int Unique, typename ReturnType>
		struct method_wrap0
		{
			static boost::function1<ReturnType, RealClass*> func;
			static VALUE wrap_method(int argc, VALUE *argv, VALUE self)
			{
				test_argc(argc, 0);
				RealClass* inst;
				Data_Get_Struct(self, RealClass, inst);
				return invoke_value_cast(func, inst);
			}
		};

		template <int Unique, typename ReturnType, typename P1>
		struct method_wrap1
		{
			static boost::function2<ReturnType, RealClass*, P1> func;
			static VALUE wrap_method(int argc, VALUE *argv, VALUE self)
			{
				test_argc(argc, 1);
				RealClass* inst;
				Data_Get_Struct(self, RealClass, inst);
				return invoke_value_cast(func, inst, from_value<P1>(argv[0]));
			}
		};

	private:
		//////////////////////////////////////////////////////////////////////
		// constructor wraps

		static VALUE wrap_ctor0(int argc, VALUE *argv, VALUE self)
		{
			test_argc(argc, 0);

			new (DATA_PTR(self)) RealClass;
			return self;
		}

		template <typename P1>
		static VALUE wrap_ctor1(int argc, VALUE *argv, VALUE self)
		{
			test_argc(argc, 1);

			new (DATA_PTR(self)) RealClass(from_value<P1>(argv[0]));
			return self;
		}

		void construct(const char* rname, VALUE module)
		{
			assert(ParentType::rubyClass_ != NULL);
			BaseClass::rubyClass_ = rb_define_class_under(module, rname, ParentType::rubyClass_);

			rb_define_singleton_method(BaseClass::rubyClass_, "new", vf_cast(wrap_allocate), -1);
		}

		static VALUE wrap_allocate(int argc, VALUE *argv, VALUE clas)
		{
			assert(clas == BaseClass::rubyClass_);
			void* data = m_allocator.allocate(sizeof(RealClass));

			VALUE newobj = Data_Wrap_Struct(BaseClass::rubyClass_, NULL, wrap_free, data);

			//hack to make all constructors be called by separate functions
			char init[15];
			sprintf(init, "initialize%d", argc);

			rb_funcall2(newobj, rb_intern(init), argc, argv);

			return newobj;
		};

		static void wrap_free(void* data)
		{
			reinterpret_cast< RealClass* >(data)->~RealClass();
			m_allocator.deallocate(reinterpret_cast< unsigned char* >(data), sizeof(RealClass));
		}

		static Allocator m_allocator;
	};

	template <typename RealClass, typename Parent, typename Allocator>
	Allocator class_<RealClass, Parent, Allocator>::m_allocator;

	template <typename RealClass, typename Parent, typename Allocator>
	template <int Unique, typename ReturnType>
	boost::function1<ReturnType, RealClass*> class_<RealClass, Parent, Allocator>::method_wrap0<Unique, ReturnType>::func;

	template <typename RealClass, typename Parent, typename Allocator>
	template <int Unique, typename ReturnType, typename P1>
	boost::function2<ReturnType, RealClass*, P1> class_<RealClass, Parent, Allocator>::method_wrap1<Unique, ReturnType, P1>::func;
} // rubybind

#define def real_def<__COUNTER__>
