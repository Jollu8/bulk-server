#pragma once

#include "handler.h"


#include <memory>
#include <iostream>
#include <boost/asio.hpp>

namespace bulk {
    using boost::asio::ip::tcp;

    using buffer_t = char;
    const int max_length = 1024;

    class Session : public std::enable_shared_from_this<bulk::Session> {
    private: // Fields
        tcp::socket m_socket_;
        buffer_t m_buffer_[max_length];
        bulk::Handler *m_handler_;

    private: // Methods
        void Do_read() {
            auto self(shared_from_this());
            m_socket_.async_read_some(
                    boost::asio::buffer(m_buffer_, max_length),
                    [this, self](boost::system::error_code error, std::size_t length) {
                        if (!error) m_handler_->Receive(m_buffer_, length);
                    }
            );
        }

    public:
        Session(tcp::socket socket, bulk::Handler *handler) :
                m_socket_(std::move(socket)),
                m_handler_(handler) {}

    public: // Destructor
        ~Session() {
            delete m_handler_;
        }

    public: // Method
        void Start() {
            Do_read();
        }

    };
}