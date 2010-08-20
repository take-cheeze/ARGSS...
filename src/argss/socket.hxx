#ifndef _ARGSS_SOCKET_HXX_
#define _ARGSS_SOCKET_HXX_

#include <argss/ruby.hxx>


namespace ARGSS
{
	namespace ASocket
	{
		VALUE& getID();

		void Init();
		void CheckDisposed(VALUE self);

		VALUE rinitialize(int argc, VALUE* argv, VALUE self);
		VALUE rdispose(VALUE self);
		VALUE rdisposedQ(VALUE self);

		VALUE rconnect(int argc, VALUE* argv, VALUE self);
		VALUE rlisten(int argc, VALUE* argv, VALUE self);
		VALUE rreceive(VALUE self);
		VALUE rsend(VALUE self, VALUE data);
		VALUE rshutdown(VALUE self);
	} // namespace ASocket
} // namespace ARGSS

#endif // _ARGSS_SOCKET_HXX_
