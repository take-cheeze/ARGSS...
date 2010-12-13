#include <boost/unordered_map.hpp>
#include <boost/smart_ptr.hpp>

#include <algorithm>
#include <utility>

#include <argss/socket.hxx>
#include "../socket.hxx"


namespace ARGSS
{
	namespace ASocket
	{
		namespace
		{
			boost::unordered_map< VALUE, boost::shared_ptr<Socket> > sockets_;
			VALUE id_;

			Socket& getSocket(VALUE self)
			{
				assert( sockets_.find(self) != sockets_.end() );
				return *sockets_.find(self)->second;
			}
			bool socketExists(VALUE self) { return sockets_.find(self) != sockets_.end(); }
		} // namespace
		VALUE& getID() { return id_; }

		void Init()
		{
			id_ = rb_define_class("Socket", rb_cObject);
			static FuncTable funcTable =
			{
				{ ARGSS_FUNC(initialize), -1 },
				{ ARGSS_FUNC(dispose), 0 }, { ARGSS_Q(disposed), 0 },
				{ ARGSS_FUNC(connect), 0 },
				{ ARGSS_FUNC(listen), -1 },
				{ ARGSS_FUNC(receive), 0 },
				{ ARGSS_FUNC(send), 1 },
				{ ARGSS_FUNC(shutdown), 0 },
			};
			defineMethods(id_, funcTable);
		}
		void CheckDisposed(VALUE self)
		{
		}

		VALUE rinitialize(int argc, VALUE* argv, VALUE self)
		{
			switch(argc) {
				case 0:
					sockets_.insert( std::make_pair( self, new Socket() ) );
					break;
				default: assert(false);
			}
			ARGSS::ARuby::AddObject(self);
			return self;
		}
		VALUE rdispose(VALUE self)
		{
			sockets_.erase(self);
			ARGSS::ARuby::RemoveObject(self);
			rb_gc();
			return self;
		}
		VALUE rdisposedQ(VALUE self)
		{
			return INT2BOOL( socketExists(self) );
		}

		VALUE rconnect(int argc, VALUE* argv, VALUE self)
		{
			switch(argc) {
				case 2:
					getSocket(self).Connect( VAL2STR(argv[0]), NUM2INT(argv[1]) );
					break;
				default: assert(false);
			}
			return self;
		}
		VALUE rlisten(int argc, VALUE* argv, VALUE self)
		{
			switch(argc) {
				case 1:
					getSocket(self).Listen( NUM2INT(argv[0]) );
					break;
				case 2:
					getSocket(self).Listen( NUM2INT(argv[0]), NUM2INT(argv[1]) );
					break;
				default: assert(false);
			}
			return self;
		}
		VALUE rreceive(VALUE self)
		{
			std::vector<uint8_t> vec = getSocket(self).Receive();
			return rb_str_new( reinterpret_cast<char const*>( &(vec[0]) ), vec.size() );
		}
		VALUE rsend(VALUE self, VALUE data)
		{
			std::vector<uint8_t> vec( RSTRING_LEN(data) );
			std::copy( RSTRING_PTR(data), RSTRING_PTR(data) + vec.size(), vec.begin() );
			getSocket(self).Send(vec);
			return self;
		}
		VALUE rshutdown(VALUE self)
		{
			getSocket(self).Shutdown();
			return self;
		}
	} // namespace ASocket
} // namespace ARGSS
