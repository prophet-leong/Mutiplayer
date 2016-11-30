#pragma once
#include <WinSock2.h>
#include <vector>
#include <string>
#include "SessionInfo.h"
using std::string;
using std::vector;
class BackSlashCommands
{
public:
	BackSlashCommands();
	~BackSlashCommands();
	void initCommands();
	int FindCommand(char *message);
	bool CommandResponse(SessionInfo* g_SessionList,int SessionIndex, int command);
	std::vector<std::string> Commands;
	std::vector<std::string> Explaination;
	void CheckForSpam(SessionInfo* g_SessionList, int SessionIndex);
	float MaxBanTime;
	bool Exit = false;
	bool SpamBan = false;
protected:
	void MessageCmd(SessionInfo* g_SessionList, int SessionIndex, int command);
	void ShowAllCmd(SessionInfo* g_SessionList, int SessionIndex, int command);
	void WhisperCmd(SessionInfo* g_SessionList, int SessionIndex, int command);
	void HelpCmd(SessionInfo* g_SessionList, int SessionIndex, int command);
	void ExitCmd(SessionInfo* g_SessionList, int SessionIndex, int command);
};

