#pragma once

#include <string>
#include <vector>

namespace Game
{
    struct Record
    {
        std::string name;
        int score = 0;
    };

    struct SessionResult
    {
        int score = 0;
        bool won = false;
    };

    class ISessionObserver
    {
    public:
        virtual ~ISessionObserver() = default;
        virtual void onSessionFinished(const SessionResult& result) = 0;
    };

    class Leaderboard : public ISessionObserver
    {
    public:
        static constexpr int StoredCount = 10;
        static constexpr int PopupCount = 5;

        explicit Leaderboard(std::string path);

        void load();
        void save() const;

        bool isRecord(int score) const;
        void add(std::string name, int score);

        void onSessionFinished(const SessionResult& result) override;
        bool hasSession() const { return mHasSession; }
        const SessionResult& session() const { return mSession; }
        bool sessionNeedsName() const;
        int commitSession(std::string name);
        void clearSession();
        int highlight() const { return mHighlight; }

        const std::vector<Record>& records() const { return mRecords; }

    private:
        void sortAndTrim();

        std::string mPath;
        std::vector<Record> mRecords;
        SessionResult mSession;
        bool mHasSession = false;
        bool mCommitted = false;
        int mHighlight = -1;
    };
}
