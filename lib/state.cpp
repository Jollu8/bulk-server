#include "state.h"
#include "handler.h"

using namespace bulk;

StateBase::StateBase(Handler *handler) : m_handler_(handler) {
}

StateEmpty::StateEmpty(Handler *handler) : StateBase(handler) {
}

void StateEmpty::CmdOpenedBracket() {
    m_handler_->PushOpenedBracket();
    m_handler_->SetState(StateBasePtr{new StateDynamic(m_handler_)});
}

void StateEmpty::CmdClosedBracket() {
    PushCmd("}");
}

void StateEmpty::CmdEof() {
    // nothing to do
}

void StateEmpty::CmdOther(const Cmd &cmd) {
    PushCmd(cmd);
}

void StateEmpty::PushCmd(const Cmd &cmd) {
    m_handler_->PushCmdStatic(cmd);
    if (m_handler_->CmdSize() == m_handler_->BulkSize()) {
        m_handler_->ProcessBulkStatic();
    } else {
        m_handler_->SetState(StateBasePtr{new StateStatic(m_handler_)});
    }
}

StateStatic::StateStatic(Handler *handler) : StateBase(handler) {
}

void StateStatic::CmdOpenedBracket() {
    m_handler_->PushOpenedBracket();
    m_handler_->SetState(StateBasePtr{new StateDynamic(m_handler_)});
}

void StateStatic::CmdClosedBracket() {
    PushCmd("}");
}

void StateStatic::CmdEof() {
    m_handler_->ProcessBulkStatic();
}

void StateStatic::CmdOther(const Cmd &cmd) {
    PushCmd(cmd);
}

void StateStatic::PushCmd(const Cmd &cmd) {
    m_handler_->PushCmdStatic(cmd);
    if (m_handler_->CmdSize() == m_handler_->BulkSize()) {
        m_handler_->ProcessBulkStatic();
    }
}

StateDynamic::StateDynamic(Handler *handler) : StateBase(handler) {
}

void StateDynamic::CmdOpenedBracket() {
    m_handler_->PushOpenedBracket();
}

void StateDynamic::CmdClosedBracket() {
    m_handler_->PopOpenedBracket();
    if (m_handler_->BracketSize() == 0) {
        m_handler_->ProcessBulkDynamic();
        m_handler_->SetState(StateBasePtr{new StateEmpty(m_handler_)});
    }
}

void StateDynamic::CmdEof() {
    // nothing to do
}

void StateDynamic::CmdOther(const Cmd &cmd) {
    m_handler_->PushCmdDynamic(cmd);
}