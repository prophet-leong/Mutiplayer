#include "BackSlashCommands.h"


BackSlashCommands::BackSlashCommands()
{
}

void BackSlashCommands::initCommands()
{
	MaxBanTime = 10.0f;
	////0
	//Commands.push_back("/message");
	//Explaination.push_back("To message everyone in the chat room");
	//1
	Commands.push_back("/showall");
	Explaination.push_back("Shows Everyone's directory in the chatroom");
	//2
	Commands.push_back("/whisper/");
	Explaination.push_back("Whisper to a user by typing /whisper/(their directory number)");
	//3
	Commands.push_back("/help");
	Explaination.push_back("You should know what this does");
	//4
	Commands.push_back("/Exit");
	Explaination.push_back("quit the chatroom");
}
void BackSlashCommands::CheckForSpam(SessionInfo* g_SessionList, int SessionIndex)
{
	if (g_SessionList[SessionIndex].currTime == 0)
	{
		time(&g_SessionList[SessionIndex].currTime);
	}
	else if(SpamBan == true)
	{
		time_t tempTime;
		time(&tempTime);
		double seconds = difftime(tempTime, g_SessionList[SessionIndex].currTime);
		if (seconds < MaxBanTime)
		{
			string temp = "Stop Spamming. You still have " + std::to_string((int)(MaxBanTime - seconds)) + " second left.";
			send(g_SessionList[SessionIndex].Socket, temp.c_str(), temp.length(), 0);
		}
		else
		{
			MaxBanTime *= 2;
			g_SessionList[SessionIndex].currTime = tempTime;
			SpamBan = false;
		}
	}
	else
	{
		time_t tempTime;
		time(&tempTime);
		double seconds = difftime(tempTime, g_SessionList[SessionIndex].currTime);
		g_SessionList[SessionIndex].currTime = tempTime;
		if (seconds < 0.1f)
		{
			SpamBan = true;
			char message[] = "Stop Spamming else you will have a temporary chat ban";
			send(g_SessionList[SessionIndex].Socket,message,strlen(message), 0);
		}
	}
}
int BackSlashCommands::FindCommand(char *message)
{
	if (message[0] == '/')
	{
		string temp = message;
		for (unsigned int i = 0; i < Commands.size(); ++i)
		{
			if (temp.find(Commands[i]) == 0)
			{
				return i+1;
			}
		}
	}
	else
	{
		return 0;
	}
	return -1;
}
bool BackSlashCommands::CommandResponse(SessionInfo* g_SessionList,int SessionIndex,int Command)
{
	if (Command == -1)
		return false;
	else
	{
		switch (Command)
		{
		case 0:
			MessageCmd(g_SessionList, SessionIndex, Command); break;
		case 1: 
			ShowAllCmd(g_SessionList, SessionIndex, Command); break;
		case 2:	
			WhisperCmd(g_SessionList, SessionIndex, Command); break;
		case 3:
			HelpCmd(g_SessionList, SessionIndex, Command); break;
		case 4:
			ExitCmd(g_SessionList, SessionIndex, Command); break;
		}
		return true;
	}
}
void BackSlashCommands::MessageCmd(SessionInfo* g_SessionList, int SessionIndex, int Command)
{
	string Msg = g_SessionList[SessionIndex].MessageBuffer;
	struct tm * timeinfo;
	timeinfo = localtime(&g_SessionList[SessionIndex].currTime);
	g_SessionList[SessionIndex].MessageSize = sprintf(g_SessionList[SessionIndex].MessageBuffer, "<%d:%d:%d> %d Says: %s", timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec, g_SessionList[SessionIndex].Socket, Msg.c_str());
	int i = 0;
	while (g_SessionList[i].Socket != 0)
	{
		send(g_SessionList[i].Socket, g_SessionList[SessionIndex].MessageBuffer, g_SessionList[SessionIndex].MessageSize, 0);
		++i;
	}
}
void BackSlashCommands::ShowAllCmd(SessionInfo* g_SessionList, int SessionIndex, int Command)
{
	string temp = "************Users Online**************\n";
	int i = 0;
	while (g_SessionList[i].Socket != 0)
	{
		temp.append(std::to_string(g_SessionList[i].Socket));
		if (i == SessionIndex)
			temp.append("<------ This is You");
		temp.append("\n");
		++i;
	}

	strcpy(g_SessionList[SessionIndex].MessageBuffer, temp.c_str());
	send(g_SessionList[SessionIndex].Socket, g_SessionList[SessionIndex].MessageBuffer, temp.length(), 0);
}
void BackSlashCommands::WhisperCmd(SessionInfo* g_SessionList, int SessionIndex, int Command)
{
	int a = Commands[Command].length();
	string temp = g_SessionList[SessionIndex].MessageBuffer;
	string address = temp.substr(a,3);
	string msgEnd = temp.substr(a + 3, g_SessionList[SessionIndex].MessageSize - a - 3) + "\n";
	struct tm * timeinfo;
	timeinfo = localtime(&g_SessionList[SessionIndex].currTime);
	g_SessionList[SessionIndex].MessageSize = sprintf(g_SessionList[SessionIndex].MessageBuffer, "<%d:%d:%d> %d whipers : %s", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, g_SessionList[SessionIndex].Socket, msgEnd.c_str());
	int i = 0;
	while (g_SessionList[i].Socket != 0)
	{
		if (std::to_string(g_SessionList[i].Socket) == address)
		{
			send(g_SessionList[i].Socket, g_SessionList[SessionIndex].MessageBuffer, g_SessionList[SessionIndex].MessageSize, 0);
			break;
		}
		else if (g_SessionList[i+1].Socket == 0)
		{
			char failmessage[] = "failed to send message\n\0";
			send(g_SessionList[SessionIndex].Socket, failmessage, strlen(failmessage), 0);
		}
		++i;
	}
	send(g_SessionList[SessionIndex].Socket, g_SessionList[SessionIndex].MessageBuffer, g_SessionList[SessionIndex].MessageSize, 0);
}
void BackSlashCommands::HelpCmd(SessionInfo* g_SessionList, int SessionIndex, int Command)
{
	string temp = "************Help menu**************\n";
	for (int i = 0; i < Explaination.size(); ++i)
	{
		temp.append(Commands[i]);
		temp.append("    ");
		temp.append(Explaination[i]);
		temp.append("\n");
	}
	strcpy(g_SessionList[SessionIndex].MessageBuffer, temp.c_str());
	send(g_SessionList[SessionIndex].Socket, g_SessionList[SessionIndex].MessageBuffer, temp.length(), 0);
}
void BackSlashCommands::ExitCmd(SessionInfo* g_SessionList, int SessionIndex, int Command)
{	
	Exit = true;
	char exitMsg[] = "You can close the window safely now";
	send(g_SessionList[SessionIndex].Socket, Commands[Command].c_str(), Commands[Command].size(), 0);
}
BackSlashCommands::~BackSlashCommands()
{
}
