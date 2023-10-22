#include "assignments.h"
#include "logger.h"
#include "state.h"

#include <iostream>
#include <stack>
#include <queue>
#include <list>

namespace bulk {
    class Handler {
    public: // Constructor
        Handler(const std::size_t &bulkSize, std::queue<bulk::Cmd> &cmdsStatic);

    public: // Methods High level logic
        void RegisterLogger(logger::LogPtr logger);

        void Receive(const char *data, std::size_t size);

        void ReceiveEof();

        void SetState(bulk::StateBasePtr state);


    public: // Size Properties
        std::size_t BulkSize() const;

        std::size_t CmdSize() const;

        std::size_t BracketSize() const;

        void PushOpenedBracket();

        void PopOpenedBracket();

        void PushCmdStatic(const bulk::Cmd &cmd);

        void PushCmdDynamic(const bulk::Cmd &cmd);

        void ProcessBulkStatic();

        void ProcessBulkDynamic();

    private: // Fields
        size_t m_bulkSize_;
        std::string m_buffer_;
        std::stack<bulk::Bracket> m_brackets_;
        std::queue<bulk::Cmd> m_cmdsDynamic_;
        std::queue<bulk::Cmd> &m_cmdsStatic_;
        std::list<logger::LogPtr> m_loggers_;
        bulk::StateBasePtr m_state_;
    private: // Methods
        void ReceiveCmd(const bulk::Cmd &cmd);

        void ProcessBulk(std::queue<bulk::Cmd> &bulk);

        static bool IsOpenedBracket(const bulk::Cmd &cmd);

        static bool IsClosedBracket(const bulk::Cmd &cmd);

        static bool IsAnyBracket(const bulk::Cmd &cmd, bulk::Bracket anyBracket);


    };

    using HandlerPtr = std::unique_ptr<bulk::Handler>;
}