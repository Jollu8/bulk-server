#pragma once

#include "assignments.h"

namespace bulk {
    class Handler;

    class StateBase {
    public: // Constructor
        StateBase(Handler *handler);

    public: // Destructor
        virtual ~StateBase() = default;

    public: // Methods
        virtual void CmdOpenedBracket() = 0;

        virtual void CmdClosedBracket() = 0;

        virtual void CmdEof() = 0;

        virtual void CmdOther(const bulk::Cmd &cmd) = 0;

    protected: // Fields
        Handler *m_handler_;
    };

    class StateEmpty : public StateBase {
    public: // Constructor
        StateEmpty(Handler *handler);

    public: // Virtual methods
        void CmdOpenedBracket() override;

        void CmdClosedBracket() override;

        void CmdEof() override;

        void CmdOther(const bulk::Cmd &cmd) override;

    private: // Method
        void PushCmd(const bulk::Cmd &cmd);
    };

    class StateStatic : public StateBase {
    public: // Constructor
        StateStatic(Handler *handler);

    public: // Methods
        void CmdClosedBracket() override;

        void CmdOpenedBracket() override;

        void CmdEof() override;

        void CmdOther(const bulk::Cmd &cmd) override;

    private: // Method
        void PushCmd(const bulk::Cmd &cmd);
    };

    class StateDynamic : public StateBase {
    public: // Constructor
        StateDynamic(Handler *handler);

    public: // Methods
        void CmdClosedBracket() override;

        void CmdOpenedBracket() override;

        void CmdEof() override;

        void CmdOther(const bulk::Cmd &cmd) override;

    public: // Using
        using StateBasePtr = std::unique_ptr<StateBase>;
    };
}