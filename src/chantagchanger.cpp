#include "chantagchanger.h"
#include "util.h"

std::string Settings::ClanTagChanger::value = "";
bool Settings::ClanTagChanger::animation = false;
bool Settings::ClanTagChanger::enable = false; // TODO find a way to go back to the "official" clan tag for the player?

std::vector<ClanTagChanger::Animation> ClanTagChanger::animations =
{
	Animation ("NOVAC",
		std::vector<ClanTagChanger::Frame>
		{
			Frame ("NO____", 700),
			Frame ("VAC___", 700),
			Frame ("ON_____", 850),
			Frame ("LINUX", 1700),
			Frame ("______", 600),
			Frame ("AimTux", 3000),
			Frame ("______", 700),
		}, ANIM_LOOP
	)
};
ClanTagChanger::Animation* ClanTagChanger::animation = &ClanTagChanger::animations[0];

void ClanTagChanger::CreateMove(CUserCmd* cmd)
{

	if (!engine->IsInGame() || !Settings::ClanTagChanger::enable)
		return;

	if (Settings::ClanTagChanger::value.size() == 0 && !Settings::ClanTagChanger::animation)
		return;

	long currentTime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()).count();
	static long timeStamp = currentTime_ms;

	if (currentTime_ms - timeStamp > ClanTagChanger::animation->GetCurrentFrame().time)
	{
		timeStamp = currentTime_ms;
		ClanTagChanger::animation->NextFrame();
	}

	if (Settings::ClanTagChanger::animation)
		SendClanTag(ClanTagChanger::animation->GetCurrentFrame().text.c_str(), "");
	else 
    {
        std::string ctWithEscapesProcessed = std::string(Settings::ClanTagChanger::value); 
        Util::StdReplaceStr(ctWithEscapesProcessed, "\\n", "\n"); // compute time impact? also, referential so i assume RAII builtin cleans it up...
		SendClanTag(ctWithEscapesProcessed.c_str(), "");
    }
}
