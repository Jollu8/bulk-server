#include "handler.h"

#include <sstream>

using namespace bulk;

Handler::Handler(const size_t &bulkSize, std::queue<Cmd> &cmdsStatic)
        : m_bulkSize_(bulkSize), m_cmdsStatic_(cmdsStatic), m_state_(StateBasePtr{new StateEmpty(this)}) {
}

void Handler::Receive(const char *data, size_t size) {
    for (auto i = 0u; i < size; i++) {
        if (data[i] == '\n') {
            ReceiveCmd(m_buffer_);
            m_buffer_.clear();
        } else {
            m_buffer_.append({data[i]});
        }
    }
}

void Handler::ReceiveCmd(const Cmd &cmd) {
    if (IsOpenedBracket(cmd)) {
        m_state_->CmdOpenedBracket();
    } else if (IsClosedBracket(cmd)) {
        m_state_->CmdClosedBracket();
    } else {
        m_state_->CmdOther(cmd);
    }
}

void Handler::ReceiveEof() {
    m_state_->CmdEof();
}

void Handler::RegisterLogger(logger::LogPtr logger) {
    m_loggers_.emplace_back(std::move(logger));
}

void Handler::SetState(StateBasePtr state) {
    m_state_ = std::move(state);
}

size_t Handler::BulkSize() const {
    return m_bulkSize_;
}

size_t Handler::CmdSize() const {
    return m_cmdsStatic_.size();
}

size_t Handler::BracketSize() const {
    return m_brackets_.size();
}

void Handler::PushOpenedBracket() {
    m_brackets_.push('{');
}

void Handler::PopOpenedBracket() {
    m_brackets_.pop();
}

void Handler::PushCmdStatic(const Cmd &cmd) {
    m_cmdsStatic_.push(cmd);
}

void Handler::PushCmdDynamic(const Cmd &cmd) {
    m_cmdsDynamic_.push(cmd);
}

void Handler::ProcessBulkStatic() {
    ProcessBulk(m_cmdsStatic_);
}

void Handler::ProcessBulkDynamic() {
    ProcessBulk(m_cmdsDynamic_);
}

void Handler::ProcessBulk(std::queue<Cmd> &bulk) {
    if (!bulk.empty()) {
        for (const auto &logger: m_loggers_) {
            logger->Process(bulk);
        }
        while (!bulk.empty()) {
            bulk.pop();
        }
    }
}

bool Handler::IsOpenedBracket(const Cmd &cmd) {
    return IsAnyBracket(cmd, '{');
}

bool Handler::IsClosedBracket(const Cmd &cmd) {
    return IsAnyBracket(cmd, '}');
}

bool Handler::IsAnyBracket(const Cmd &cmd, Bracket anyBracket) {
    if (cmd.size() == 1) {
        return cmd.at(0) == anyBracket;
    } else {
        return false;
    }
}