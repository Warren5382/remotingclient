#ifndef PULLSYSFLAG_H
#define PULLSYSFLAG_H


class PullSysFlag {
private:
	static int FLAG_COMMIT_OFFSET;
	static int FLAG_SUSPEND;
	static int FLAG_SUBSCRIPTION;

public:
	static int buildSysFlag(bool commitOffset, bool suspend, bool subscription) {
			int flag = 0;

			if (commitOffset) {
				flag |= FLAG_COMMIT_OFFSET;
			}

			if (suspend) {
				flag |= FLAG_SUSPEND;
			}

			if (subscription) {
				flag |= FLAG_SUBSCRIPTION;
			}

			return flag;
	};


	static int clearCommitOffsetFlag(int sysFlag) {
		return sysFlag & (~FLAG_COMMIT_OFFSET);
	};


	static bool hasCommitOffsetFlag(int sysFlag) {
		return (sysFlag & FLAG_COMMIT_OFFSET) == FLAG_COMMIT_OFFSET;
	};


	static bool hasSuspendFlag(int sysFlag) {
		return (sysFlag & FLAG_SUSPEND) == FLAG_SUSPEND;
	};


	static bool hasSubscriptionFlag(int sysFlag) {
		return (sysFlag & FLAG_SUBSCRIPTION) == FLAG_SUBSCRIPTION;
	};
};

#endif