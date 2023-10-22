#pragma once

#include "lib/logger.h"
#include "lib/session.h"

#include <memory>

namespace bulk {
    class Server {
    private: // Fields
        tcp::acceptor m_acceptor_;
        std::size_t m_bulkSize_;
        logger::LogPtr m_console_;
        logger::LogPtr m_logfile_;
        std::queue<bulk::Cmd> m_cmdStatic_;
    private: // Methods
        void Do_accept() {
            m_acceptor_.async_accept(
                    [this](boost::system::error_code error, tcp::socket socket) {
                        if (!error) {
                            Handler *handler{new Handler(m_bulkSize_, m_cmdStatic_)};
                            handler->RegisterLogger(m_console_);
                            handler->RegisterLogger(m_logfile_);
                            std::make_shared<Session>(std::move(socket), handler)->Start();
                        }
                        Do_accept();

                    }
            );
        }

    public: // Constructor
        Server(boost::asio::io_context &io_context, short port, std::size_t bulkSize) :
                m_acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
                m_bulkSize_(bulkSize),
                m_console_(new logger::Console(std::cout)),
                m_logfile_(new logger::LogFile) {
            Do_accept();
        }

    };
}