
#ifndef _RELAYER_H_
#define _RELAYER_H_

#include <functional>

#include <queue>

#include <boost/asio.hpp>

#include "relay.h"

namespace relay
{
	namespace asio = boost::asio;
	namespace ip = asio::ip;
	using ip::tcp;

	class relayer
	{
		public:
			// constructor
			relayer(tcp::socket && A, tcp::socket && B, size_t buffer_size = 8192);
			relayer(const relayer &) = delete;

			// copy assignment
			void operator=(const relayer &) = delete;

			// destructor
			~relayer();

			// statistics
			std::pair<size_t, size_t> stat() const noexcept;

			// access sockets
			tcp::socket & get_A() noexcept;
			tcp::socket & get_B() noexcept;
		private:
			size_t buffer_size;
			bool shutdown_AB, shutdown_BA;
			tcp::socket socketA;
			tcp::socket socketB;
			std::queue<std::pair<std::shared_ptr<buffer_t>, size_t>> dataA;
			std::queue<std::pair<std::shared_ptr<buffer_t>, size_t>> dataB;
			bool writingA, writingB;
			size_t bytesA, bytesB;

			void handle_read(std::shared_ptr<buffer_t> buffer, bool from_A,
							 const boost::system::error_code &ec, size_t bytes_transferred);
			void handle_write(std::shared_ptr<buffer_t> buffer, bool to_A,
							  const boost::system::error_code &ec, size_t bytes_transferred);
			void check_and_write();
	};
}

#endif