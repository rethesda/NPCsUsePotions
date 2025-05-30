#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "Settings.h"
#include "Distribution.h"
#include <tuple>
#include <utility>

using ActorInfoPtr = std::weak_ptr<ActorInfo>;

/// <summary>
/// Provides generic functions
/// </summary>
class Utility
{
public:

	#pragma region Comparison
	struct EqualsWeakPtrActorInfo
	{
		bool operator()(const ActorInfoPtr& lhs, const ActorInfoPtr& rhs)
		{
			return !lhs.owner_before(rhs) && !rhs.owner_before(lhs);
		}
	};

	template<typename T, typename U> 
	bool EqualsSharedPtr(const std::weak_ptr<T>& t, const std::weak_ptr<U>& u)
	{
		return !t.owner_before(u) && !u.owner_before(t);
	}

	/* template <typename T, typename U>
	inline bool EqualsSharedPtr(const std::weak_ptr<T>& t, const std::shared_ptr<U>& u)
	{
		return !t.owner_before(u) && !u.owner_before(t);
	}*/
	#pragma endregion

	#pragma region SortingFunctions
	// comparator used to sort magnitude - duration - AlchemyItem* lists for maximum magnitude descending
	/// <summary>
	/// Compares the magnitude of two alchemy items
	/// </summary>
	/// <param name="first"></param>
	/// <param name="second"></param>
	/// <returns></returns>
	static bool SortMagnitude(std::tuple<float, int, RE::AlchemyItem*, AlchemicEffect> first, std::tuple<float, int, RE::AlchemyItem*, AlchemicEffect> second);

	// comparator used to sort magnitude - duration - AlchemyItem* lists for minimal amount of effects and max magnitude
	/// <summary>
	/// Compares the magnitude of two alchemy items
	/// </summary>
	/// <param name="first"></param>
	/// <param name="second"></param>
	/// <returns></returns>
	static bool SortPotion(std::tuple<float, int, RE::AlchemyItem*, AlchemicEffect> first, std::tuple<float, int, RE::AlchemyItem*, AlchemicEffect> second);

	// comparator used to sort magnitude - duration - AlchemyItem* lists for no restorative effects, minimal amount of effects and max magnitude
	/// <summary>
	/// Compares the magnitude of two alchemy items
	/// </summary>
	/// <param name="first"></param>
	/// <param name="second"></param>
	/// <returns></returns>
	static bool SortFortify(std::tuple<float, int, RE::AlchemyItem*, AlchemicEffect> first, std::tuple<float, int, RE::AlchemyItem*, AlchemicEffect> second);
	#pragma endregion

	/// <summary>
	/// Returns a string showing [val] as Hexadecimal number
	/// </summary>
	/// <param name="val"></param>
	/// <returns></returns>
	static std::string GetHex(long val)
	{
		std::stringstream ss;
		ss << std::hex << val;
		return ss.str();
	}
	/// <summary>
	/// Returns a string showing [val] as Hexadecimal number
	/// </summary>
	/// <param name="val"></param>
	/// <returns></returns>
	static std::string GetHex(uint64_t val)
	{
		std::stringstream ss;
		ss << std::hex << val;
		return ss.str();
	}
	/// <summary>
	/// Returns a string showing [val] as Hexadecimal number
	/// </summary>
	/// <param name="val"></param>
	/// <returns></returns>
	static std::string GetHex(uint32_t val)
	{
		std::stringstream ss;
		ss << std::hex << val;
		return ss.str();
	}
	/// <summary>
	/// Returns a string showing [val] as Hexadecimal number
	/// </summary>
	/// <param name="val"></param>
	/// <returns></returns>
	static std::string GetHex(int val)
	{
		std::stringstream ss;
		ss << std::hex << val;
		return ss.str();
	}
	/// <summary>
	/// Returns a string showing [val] as Hexadecimal number with padding
	/// </summary>
	/// <param name="val"></param>
	/// <returns></returns>
	static std::string GetHexFill(uint32_t val)
	{
		std::stringstream ss;
		ss << std::setw(16) << std::hex << std::setfill('0') << val;
		return ss.str();
	}
	/// <summary>
	/// Returns a string showing [val] as Hexadecimal number with padding
	/// </summary>
	/// <param name="val"></param>
	/// <returns></returns>
	static std::string GetHexFill(uint64_t val)
	{
		std::stringstream ss;
		ss << std::setw(16) << std::hex << std::setfill('0') << val;
		return ss.str();
	}

	/// <summary>
	/// Returns a string representing the given form
	/// </summary>
	/// <param name="form"></param>
	/// <returns></returns>
	template <class T>
	static std::string PrintForm(T* form)
	{
		if (form == nullptr || form->GetFormID() == 0 || Logging::EnableGenericLogging == false)
			return "None";
		std::string plugin = "";
		if ((form->GetFormID() & 0xFF000000) != 0xFE000000) {
			plugin = Settings::pluginnames[(form->GetFormID() >> 24)];
		} else
			plugin = Settings::pluginnames[256 + (((form->GetFormID() & 0x00FFF000)) >> 12)];

		return std::string("[") + typeid(T).name() + "<" + Utility::GetHex(form->GetFormID()) + "><" + form->GetName() + "><" + plugin + ">]";
	}
	static std::string PrintForm(ActorInfo* acinfo);
	static std::string PrintForm(std::shared_ptr<ActorInfo> const& acinfo);
	static std::string PrintForm(std::weak_ptr<ActorInfo> acweak);

	/// <summary>
	/// Returns a string representing the given form
	/// </summary>
	/// <param name="form"></param>
	/// <returns></returns>
	template <class T>
	static std::string PrintFormNonDebug(T* form)
	{
		if (form == nullptr || form->GetFormID() == 0 || Logging::EnableGenericLogging == false)
			return "None";
		std::string plugin = "";
		if ((form->GetFormID() & 0xFF000000) != 0xFE000000) {
			plugin = Settings::pluginnames[(form->GetFormID() >> 24)];
		} else
			plugin = Settings::pluginnames[256 + (((form->GetFormID() & 0x00FFF000)) >> 12)];

		return std::string("[") + typeid(T).name() + "<" + Utility::GetHex(form->GetFormID()) + "><" + form->GetName() + "><" + plugin + ">]";
	}

	/// <summary>
	/// Converts all symbols in a string into lower case.
	/// </summary>
	/// <param name="s"></param>
	/// <returns></returns>
	static std::string ToLower(std::string s)
	{
		std::transform(s.begin(), s.end(), s.begin(),
			[](unsigned char c) { return (unsigned char)std::tolower(c); }  // correct
		);
		return s;
	}

	/// <summary>
	/// converts an ActorStrength into a string
	/// </summary>
	/// <param name="acs"></param>
	/// <returns></returns>
	static std::string ToString(ActorStrength acs);

	/// <summary>
	/// Converts an ItemStrength into a string
	/// </summary>
	/// <param name="is"></param>
	/// <returns></returns>
	static std::string ToString(ItemStrength is);

	/// <summary>
	/// Converts an AlchemicEffect to a String
	/// </summary>
	/// <param name="ae"></param>
	/// <returns></returns>
	static std::string ToString(AlchemicEffect ae);

	/// <summary>
	/// Returns a string representation of a distribution
	/// </summary>
	/// <param name="distribution"></param>
	/// <returns></returns>
	static std::string PrintDistribution(std::vector<std::tuple<int, AlchemicEffect>> distribution);

	/// <summary>
	/// Returns a string representation of an effect map
	/// </summary>
	/// <param name="distribution"></param>
	/// <returns></returns>
	static std::string PrintEffectMap(std::map<AlchemicEffect, float> effectMap);

	/// <summary>
	/// Splits a string at a delimiter, optionally removes empty results, and optionally respects escaped sequences
	/// </summary>
	/// <param name="str">input to split</param>
	/// <param name="delimiter">delimiter at which to split</param>
	/// <param name="removeEmpty">whether to remove empty strings</param>
	/// <param name="escape">whether to respect escaped sequences</param>
	/// <param name="escapesymbol">symbol that marks escaped sequences, sequences need to be surrounded by this symbol</param>
	/// <returns></returns>
	static std::vector<std::string> SplitString(std::string str, char delimiter, bool removeEmpty, bool escape = false, char escapesymbol = '\"', bool allowdisableescape = false, char disblechar = '\\');

	/// <summary>
	/// Splits a string at a delimiter, optionally removes empty results, and optionally respects escaped sequences
	/// </summary>
	/// <param name="str">input to split</param>
	/// <param name="delimiter">delimiter at which to split</param>
	/// <param name="removeEmpty">whether to remove empty strings</param>
	/// <param name="escape">whether to respect escaped sequences</param>
	/// <param name="escapesymbol">symbol that marks escaped sequences, sequences need to be surrounded by this symbol</param>
	/// <returns></returns>
	static std::vector<std::string> SplitString(std::string str, std::string delimiter, bool removeEmpty, bool escape = false, char escapesymbol = '\"');

	/// <summary>
	/// Evaluates whether the char at position [position] is escaped
	/// </summary>
	/// <param name="str">input to search</param>
	/// <param name="escapesymbol">symbol that marks escaped sequences, sequences need to be surrounded by this symbol</param>
	/// <param name="position">the position to check</param>
	/// <returns></returns>
	static bool IsEscaped(std::string str, int32_t position, char escapesymbol = '\"')
	{
		bool escaped = false;
		int32_t count = 0;
		for (char c : str) {
			//logdebug("Char: {}\t Window: {}\t Wsize: {}\t Wmaxsize: {}\t tmp: {}", c, slide.GetWindow(), slide.size, slide.maxsize, tmp);
			if (escaped == true) {
				// escaped sequence.
				if (c == escapesymbol) {
					escaped = !escaped;
				}
			} else {
				if (c == escapesymbol) {
					// we are at the beginning of an escaped sequence
					escaped = !escaped;
				}
			}
			if (count == position)
				return escaped;
			count++;
		}
		return false;
	}

	/// <summary>
	/// Removes whitespace from an input in-place, optionally respects escaped sequences
	/// </summary>
	static std::string& RemoveWhiteSpaces(std::string& str, char escape, bool removetab = false, bool allowdisableescape = false, char disablechar = '\\');

	/// <summary>
	/// Removes Symbols from an input in-place, optionally respects escaped sequences
	/// </summary>
	static std::string& RemoveSymbols(std::string& str, char symbol, bool enableescape = false, char escape = '\"');

	/// <summary>
	/// Removes Symbols from an input in-place, optionally doesn't remove escaped characters
	/// </summary>
	static std::string& RemoveSymbols(std::string& str, char symbol, char disablechar);

	/// <summary>
	/// Counts the occurences of the given symbol, respecting escaped sequences
	/// </summary>
	static int32_t CountSymbols(std::string str, char symbol, char escaped1, char escaped2)
	{
		int32_t count = 0;
		bool escape = false;
		for (char& c : str) {
			// escaped strings are marked by, e.g., " or ' and everything between two of them should not be counted
			// we need to check for both since one string could be marked with ", while another one could use '
			// this means that something thats invalid in python "usdzfgf' is counted as valid.
			if (c == escaped1 || c == escaped2)
				escape = !escape;
			if (escape == false)
				if (c == symbol)
					count++;
		}
		return count;
	}

	static std::vector<std::pair<char, int32_t>> GetSymbols(std::string str);


	/// <summary>
	/// The current combat style of an actor
	/// </summary>
	enum class CurrentCombatStyle
	{
		Spellsword = 0x1, // combination spell and onehanded
		OneHandedShield = 0x2,
		TwoHanded = 0x4,
		MagicDestruction = 0x8,
		MagicConjuration = 0x10,
		MagicAlteration = 0x20,
		MagicIllusion = 0x40,
		MagicRestoration = 0x80,
		OneHanded = 0x100,
		Ranged = 0x200,
		DualWield = 0x400,
		Staffsword = 0x800,  // combination staff and onehanded
		HandToHand = 0x1000,
		Mage = 0x2000,
		DualStaff = 0x4000,
		Staff = 0x8000,
		MagicDamageFire = 0x100000,
		MagicDamageShock = 0x200000,
		MagicDamageFrost = 0x400000,
	};

	/// <summary>
	/// Converts a CurrentCombatStyle value into a string
	/// </summary>
	/// <param name="style">CurrentCombatStyle value to convert</param>
	/// <returns>String representing [style]</returns>
	static std::string ToStringCombatStyle(uint32_t style);

	/// <summary>
	/// Retrieves data about the current equiped items and spells of an actor
	/// </summary>
	/// <param name="actor">the actor to check</param>
	/// <returns>A linear combination of CurrentCombatStyle, representing the combat data of an actor</returns>
	static uint32_t GetCombatData(RE::Actor* actor);

	enum class CurrentArmor
	{
		None = 0,
		HeavyArmor = 1 << 0,
		LightArmor = 1 << 1,
		Clothing = 1 << 2,
	};

	/// <summary>
	/// Returns which armor types are worn by a given npc.
	/// </summary>
	/// <param name="actor">The NPC to check.</param>
	/// <returns>Linear combination of values of CurrentArmor</returns>
	static uint32_t GetArmorData(RE::Actor* actor);

	/// <summary>
	/// Returns the type of item, specific weapon type etc
	/// </summary>
	/// <param name="form"></param>
	/// <returns></returns>
	static int GetItemType(RE::TESForm* form);

	#pragma region Parsing

	/// <summary>
	/// Parses a string into a vector of int (array of int)
	/// </summary>
	/// <param name="line">string to parse</param>
	/// <returns>vector of int (array of int)</returns>
	static std::vector<int> ParseIntArray(std::string line);

	/// <summary>
	/// returns a TESForm* from various inputs
	/// </summary>
	/// <param name="datahandler">datahandler to get data from</param>
	/// <param name="formid">id or partial id of item (may be 0, if editorid is set)</param>
	/// <param name="pluginname">name of the plugin the item is included (may be the empty string, if item is in the basegame, or editorid is given)</param>
	/// <param name="editorid">editorid of the item, defaults to empty string</param>
	/// <returns></returns>
	static RE::TESForm* GetTESForm(RE::TESDataHandler* datahandler, RE::FormID formid, std::string pluginname, std::string editorid = "");

	/// <summary>
	/// Parses objects for distribution rules from a string input
	/// </summary>
	/// <param name="input">the string to parse</param>
	/// <param name="error">will be overwritten with [true] if an error occurs</param>
	/// <param name="file">the relative path of the file that contains the string</param>
	/// <param name="line">the line in the file that contains the string</param>
	/// <returns>a vector of parsed and validated objects</returns>
	static std::vector<std::tuple<Distribution::AssocType, RE::FormID, RE::TESForm*>> ParseAssocObjects(std::string input, bool& error, std::string file, std::string line, int& totalobjects);

	/// <summary>
	/// Parses objects for distribution rules from a string input with an optional chance for items
	/// </summary>
	/// <param name="input">the string to parse</param>
	/// <param name="error">will be overwritten with [true] if an error occurs</param>
	/// <param name="file">the relative path of the file that contains the string</param>
	/// <param name="line">the line in the file that contains the string</param>
	/// <returns>a vector of parsed and validated objects and their chances</returns>
	static std::vector<std::tuple<Distribution::AssocType, RE::FormID, int32_t, CustomItemFlag, int8_t, bool, std::vector<std::tuple<uint64_t, uint32_t, std::string>>, std::vector<std::tuple<uint64_t, uint32_t, std::string>>, bool>> ParseCustomObjects(std::string input, bool& error, std::string file, std::string line);

	/// <summary>
	/// Returns an AssocType for the given RE::FormType. If the RE::FormType is not supported, [valid] is set to true
	/// </summary>
	/// <param name="type">RE::FormType to convert</param>
	/// <param name="valid">Overridable value, which is set to true if [type] is not supported.</param>
	/// <returns>AssocType associated with [type]</returns>
	static Distribution::AssocType MatchValidFormType(RE::FormType type, bool& valid);

	/// <summary>
	/// Parses AlchemyEffects from an input string.
	/// </summary>
	/// <param name="input">string to parse</param>
	/// <param name="error">Overrisable value, which is set to true if there is an error during parsing.</param>
	/// <returns>A vector of AlchemyEffects, Weights, and max occurences</returns>
	static std::vector<std::tuple<AlchemicEffect, float /*weight*/, int /*max*/>> ParseAlchemyEffects(std::string input, bool& error);

	/// <summary>
	/// parses alchemy effects from an input string for Rule version 3
	/// </summary>
	/// <param name="input"></param>
	/// <param name="preset"></param>
	static bool ParseAlchemyEffects(std::string input, Distribution::EffectPreset* preset);

	/// <summary>
	/// Computes a distribution from an effectmap.
	/// </summary>
	/// <param name="effectmap">effectmap containing effects and weights which will be translated into the distribution</param>
	/// <param name="range">range the distribution chances are computed for</param>
	/// <returns>Weighted Distribution</returns>
	static std::vector<std::tuple<int, AlchemicEffect>> GetDistribution(std::vector<std::tuple<AlchemicEffect, float, int>> effectmap, int range, bool chance = false);

	/// <summary>
	/// Computes a distribution from a unified effect map
	/// <param name="map">unified effect map the distribution is calculated from</param>
	/// <param name="range">range the distribution chances are computed for</param>
	/// </summary>
	static std::vector<std::tuple<int, AlchemicEffect>> GetDistribution(std::map<AlchemicEffect, float> map, int range, bool chance = false);

	/// <summary>
	/// Calculates a unified effect map, that contains at most one entry per AlchemyEffect present
	/// </summary>
	/// <param name="effectmap">effectmap containing effects and weights that shal be unified</param>
	/// <returns>map with alchemyeffects and their weights</returns>
	static std::map<AlchemicEffect, float> UnifyEffectMap(std::vector<std::tuple<AlchemicEffect, float, int>> effectmap);

	/// <summary>
	/// Sums the Alchemyeffects in [list]
	/// </summary>
	/// <param name="list">list with AlchemyEffects to sum</param>
	/// <returns>Combined value with all Alchemyeffects</returns>
	static AlchemicEffect SumAlchemyEffects(std::vector<std::tuple<int, AlchemicEffect>> list, bool chance = false);

	/// <summary>
	/// Checks whether poison can be applied to the weapons of an actor
	/// </summary>
	/// <param name="actor">Actor to check</param>
	/// <returns>Whether poison can be applied to the actors weapons</returns>
	static bool CanApplyPoison(std::shared_ptr<ActorInfo> const& actor);

	/// <summary>
	/// Returns whether an NPC has poison applied to their weapons, and the poison applied
	/// </summary>
	/// <param name="actor">Actor to check</param>
	/// <param name="pois">Poison found [out]</param>
	/// <returns></returns>
	static bool GetAppliedPoison(RE::Actor* actor, RE::ExtraPoison*& pois);

	/// <summary>
	/// Verifies that acinfo is a valid object
	/// <param name="acinfo">ActorInfo to verify</param>
	/// </summary>
	static bool VerifyActorInfo(std::shared_ptr<ActorInfo> const& acinfo);

	/// <summary>
	/// Returns whether an actor is valid and safe to work with
	/// </summary>
	/// <param name="actor">Actor to validate</param>
	/// <returns></returns>
	static bool ValidateActor(RE::Actor* actor);

	/// <summary>
	/// Returns whether a form is valid and safe to work with
	/// </summary>
	/// <param name="form">Form to validate</param>
	/// <returns></returns>
	template<class T>
	static bool ValidateForm(T* form)
	{
		if (form == nullptr || form->GetFormID() == 0 || form->formFlags & RE::TESForm::RecordFlags::kDeleted)
			return false;
		return true;
	}

	/// <summary>
	/// Extracts template information from an Actor
	/// </summary>
	/// <param name="npc"></param>
	/// <returns></returns>
	static Misc::NPCTPLTInfo ExtractTemplateInfo(RE::Actor* actor);
	/// <summary>
	/// Extracts template information from a NPC
	/// </summary>
	/// <param name="npc"></param>
	/// <returns></returns>
	static Misc::NPCTPLTInfo ExtractTemplateInfo(RE::TESNPC* npc);
	/// <summary>
	/// Exctracts template information from a Leveled Character
	/// </summary>
	/// <param name="lvl"></param>
	/// <returns></returns>
	static Misc::NPCTPLTInfo ExtractTemplateInfo(RE::TESLevCharacter* lvl);
	#pragma endregion


	#pragma region Mods

	class Mods
	{
	public:
		/// <summary>
		/// Returns the pluginname the form is defined in
		/// </summary>
		static std::string GetPluginName(RE::TESForm* form);

		/// <summary>
		/// Returns the pluginname the form is defined in
		/// </summary>
		static std::string GetPluginNameFromID(RE::FormID formid);

		/// <summary>
		/// Returns the pluginname corresponding to the pluginIndex
		/// </summary>
		static std::string GetPluginName(uint32_t pluginIndex);

		/// <summary>
		/// Returns the plugin index of the given plugin [returns 0x1 on failure]
		/// </summary>
		/// <param name="pluginname"></param>
		/// <returns></returns>
		static uint32_t GetPluginIndex(std::string pluginname);

		/// <summary>
		/// Returns the plugin index of the given form [returns 0x1 on failure]
		/// </summary>
		/// <param name="pluginname"></param>
		/// <returns></returns>
		static uint32_t GetPluginIndex(RE::TESForm* form);

		/// <summary>
		/// Returns the formid with the plugin index
		/// </summary>
		static uint32_t GetIndexLessFormID(RE::TESForm* form);

		/// <summary>
		/// Returns the formid with the plugin index
		/// </summary>
		static uint32_t GetIndexLessFormID(RE::FormID formid);

		/// <summary>
		/// Returns a vector with all forms of the given type in the plugin
		/// </summary>
		/// <typeparam name="T">form type</typeparam>
		/// <param name="pluginname">name of the plugin</param>
		/// <returns></returns>
		template <class T>
		static std::vector<T*> GetFormsInPlugin(std::string pluginname)
		{
			auto datahandler = RE::TESDataHandler::GetSingleton();
			const RE::TESFile* file = nullptr;
			uint32_t pindex = Utility::Mods::GetPluginIndex(pluginname);
			std::vector<T*> ret;
			if (pindex == 0x1) // if mod cannot be found return
				return ret;
			if ((pindex & 0x00FFF000) != 0)  // light mod
				file = datahandler->LookupLoadedLightModByIndex((uint16_t)((pindex & 0x00FFF000) >> 12));
			else  // normal mod
				file = datahandler->LookupLoadedModByIndex((uint8_t)(pindex >> 24));
			if (file != nullptr) {
				uint32_t mask = 0;
				uint32_t index = 0;
				if (file->IsLight()) {
					mask = 0xFFFFF000;
					index = file->GetPartialIndex() << 12;
				} else {
					mask = 0xFF000000;
					index = file->GetPartialIndex() << 24;
				}
				auto forms = datahandler->GetFormArray<T>();
				for (int i = 0; i < (int)forms.size(); i++) {
					if ((forms[i]->GetFormID() & mask) == index)
						ret.push_back(forms[i]);
				}
			}
			return ret;
		}
	};

	#pragma endregion
};
