#include "ActorManipulation.h"

using AlchemyEffect = Settings::AlchemyEffect;

#define ConvAlchULong(x) static_cast<uint64_t>(Settings::ConvertToAlchemyEffect(x))
#define ULong(x) static_cast<uint64_t>(x)

//
std::tuple<bool, float, int, Settings::AlchemyEffect> ACM::HasAlchemyEffect(RE::AlchemyItem* item, uint64_t alchemyEffect)
{
	LOG_4("[HasAlchemyEffect] begin");
	if (item && !(item->IsPoison() && !(item->IsFood()))) {
		RE::EffectSetting* sett = nullptr;
		LOG_4("[HasAlchemyEffect] found medicine");
		if ((item->avEffectSetting) == nullptr && item->effects.size() == 0) {
			LOG_4("[HasAlchemyEffect] end fail");
			return { false, 0.0f, 0, AlchemyEffect::kNone};
		}
		uint64_t out = 0;
		bool found = false;
		float mag = 0;
		int dur = 0;
		if (item->effects.size() > 0) {
			for (uint32_t i = 0; i < item->effects.size(); i++) {
				sett = item->effects[i]->baseEffect;
				if (sett && (out = ConvAlchULong(sett->data.primaryAV) & alchemyEffect) > 0) {
					found = true;
					mag = item->effects[i]->effectItem.magnitude;
					dur = item->effects[i]->effectItem.duration;
					break;
				}
			}
		} else {
			RE::MagicItem::SkillUsageData err;
			item->GetSkillUsageData(err);
			if ((out = ConvAlchULong(item->avEffectSetting->data.primaryAV) & alchemyEffect) > 0) {
				found = true;
				mag = err.magnitude;
			}
		}
		if (found) {
			LOG_4("[HasAlchemyEffect] end success");
			return { true, mag, dur, static_cast<AlchemyEffect>(out) };
		}
		LOG_4("[HasAlchemyEffect] end fail");
		return { false, 0.0f, 0, AlchemyEffect::kNone };
	}
	LOG_4("[HasAlchemyEffect] end fail");
	return { false, 0.0f, 0, AlchemyEffect::kNone };
}

std::list<std::tuple<float, int, RE::AlchemyItem*, Settings::AlchemyEffect>> ACM::GetMatchingPotions(RE::Actor* actor, uint64_t alchemyEffect)
{
	std::list<std::tuple<float, int, RE::AlchemyItem*, Settings::AlchemyEffect>> ret{};
	auto itemmap = actor->GetInventory();
	auto iter = itemmap.begin();
	auto end = itemmap.end();
	RE::AlchemyItem* item = nullptr;
	LOG_3("[GetMatchingItems] trying to find potion");
	while (iter != end && alchemyEffect!= 0) {
		item = iter->first->As<RE::AlchemyItem>();
		LOG_4("[GetMatchingItems] checking item");
		if (item && !(item->IsPoison() && !(item->IsFood()))) {
			LOG_4("[GetMatchingItems] found medicine");
			if (auto res = HasAlchemyEffect(item, alchemyEffect); std::get<0>(res)) {
				ret.insert(ret.begin(), { std::get<1>(res), std::get<2>(res), item, std::get<3>(res) });
			}
		}
		iter++;
	}
	return ret;
}

std::list<std::tuple<float, int, RE::AlchemyItem*, Settings::AlchemyEffect>> ACM::GetMatchingPoisons(RE::Actor* actor, uint64_t alchemyEffect)
{
	std::list<std::tuple<float, int, RE::AlchemyItem*, Settings::AlchemyEffect>> ret{};
	auto itemmap = actor->GetInventory();
	auto iter = itemmap.begin();
	auto end = itemmap.end();
	RE::AlchemyItem* item = nullptr;
	LOG_3("[GetMatchingItems] trying to find potion");
	while (iter != end && alchemyEffect!= 0) {
		item = iter->first->As<RE::AlchemyItem>();
		LOG_4("[GetMatchingItems] checking item");
		if (item->IsPoison()) {
			LOG_4("[GetMatchingItems] found medicine");
			if (auto res = HasAlchemyEffect(item, alchemyEffect); std::get<0>(res)) {
				ret.insert(ret.begin(), { std::get<1>(res), std::get<2>(res), item, std::get<3>(res) });
			}
		}
		iter++;
	}
	return ret;
}

std::list<std::tuple<float, int, RE::AlchemyItem*, Settings::AlchemyEffect>> ACM::GetMatchingFood(RE::Actor* actor, uint64_t alchemyEffect)
{
	std::list<std::tuple<float, int, RE::AlchemyItem*, Settings::AlchemyEffect>> ret{};
	auto itemmap = actor->GetInventory();
	auto iter = itemmap.begin();
	auto end = itemmap.end();
	RE::AlchemyItem* item = nullptr;
	LOG_3("[GetMatchingItems] trying to find potion");
	while (iter != end && alchemyEffect!= 0) {
		item = iter->first->As<RE::AlchemyItem>();
		LOG_4("[GetMatchingItems] checking item");
		if (item->IsFood()) {
			LOG_4("[GetMatchingItems] found medicine");
			if (auto res = HasAlchemyEffect(item, alchemyEffect); std::get<0>(res)) {
				ret.insert(ret.begin(), { std::get<1>(res), std::get<2>(res), item, std::get<3>(res) });
			}
		}
		iter++;
	}
	return ret;
}

std::tuple<int, Settings::AlchemyEffect, std::list<std::tuple<float, int, RE::AlchemyItem*, Settings::AlchemyEffect>>> ACM::ActorUsePotion(RE::Actor* _actor, uint64_t alchemyEffect)
{
	auto begin = std::chrono::steady_clock::now();
	// if no effect is specified, return
	if (alchemyEffect == 0) {
		return { -1, AlchemyEffect::kNone, std::list<std::tuple<float, int, RE::AlchemyItem*, Settings::AlchemyEffect>>{} };
	}
	auto itemmap = _actor->GetInventory();
	auto iter = itemmap.begin();
	auto end = itemmap.end();
	//RE::EffectSetting* sett = nullptr;
	LOG_2("[ActorUsePotion] trying to find potion");
	auto ls = GetMatchingPotions(_actor, alchemyEffect);
	ls.sort(Utility::SortMagnitude);
	// got all potions the actor has sorted by magnitude.
	// now use the one with the highest magnitude;
	return ActorUsePotion(_actor, ls);
}

std::tuple<int, Settings::AlchemyEffect, std::list<std::tuple<float, int, RE::AlchemyItem*, Settings::AlchemyEffect>>> ACM::ActorUsePotion(RE::Actor* _actor, std::list<std::tuple<float, int, RE::AlchemyItem*, Settings::AlchemyEffect>> &ls)
{
	if (ls.size() > 0) {
		LOG_2("[ActorUsePotion] Drink Potion");
		if (Settings::_CompatibilityMode || Settings::_CompatibilityPotionAnimation) {
			LOG_3("[ActorUsePotion] Compatibility Mode")
			SKSE::ModCallbackEvent* ev = new SKSE::ModCallbackEvent();
			ev->eventName = RE::BSFixedString("NPCsDrinkPotionActorInfo");
			ev->strArg = RE::BSFixedString("");
			ev->numArg = 0.0f;
			ev->sender = _actor;
			SKSE::GetModCallbackEventSource()->SendEvent(ev);
			ev = new SKSE::ModCallbackEvent();
			ev->eventName = RE::BSFixedString("NPCsDrinkPotionEvent");
			ev->strArg = RE::BSFixedString("");
			ev->numArg = 0.0f;
			ev->sender = std::get<2>(ls.front());
			SKSE::GetModCallbackEventSource()->SendEvent(ev);
		} else {
			RE::ExtraDataList* extra = new RE::ExtraDataList();
			extra->SetOwner(_actor);
			RE::ActorEquipManager::GetSingleton()->EquipObject(_actor, std::get<2>(ls.front()), extra);
			//_actor->DrinkPotion(std::get<1>(ls.front()), extra);
		}
		auto val = ls.front();
		ls.pop_front();
		return { std::get<1>(ls.front()), std::get<3>(ls.front()), ls };
	}
	return { -1, AlchemyEffect::kNone, ls };
}

std::pair<int, Settings::AlchemyEffect> ACM::ActorUseFood(RE::Actor* _actor, uint64_t alchemyEffect)
{
	auto begin = std::chrono::steady_clock::now();
	// if no effect is specified, return
	if (alchemyEffect== 0) {
		return { -1, AlchemyEffect::kNone };
	}
	auto itemmap = _actor->GetInventory();
	auto iter = itemmap.begin();
	auto end = itemmap.end();
	//RE::EffectSetting* sett = nullptr;
	LOG_2("[ActorUseFood] trying to find food");
	auto ls = GetMatchingFood(_actor, alchemyEffect);
	ls.sort(Utility::SortMagnitude);
	// got all potions the actor has sorted by magnitude.
	// now use the one with the highest magnitude;
	if (ls.size() > 0) {
		LOG_2("[ActorUseFood] Use Food");
		if (Settings::_CompatibilityMode) {
			LOG_3("[ActorUseFood] Compatibility Mode");
			// use same event as for potions, since it takes a TESForm* and works for anything
			SKSE::ModCallbackEvent* ev = new SKSE::ModCallbackEvent();
			ev->eventName = RE::BSFixedString("NPCsDrinkPotionActorInfo");
			ev->strArg = RE::BSFixedString("");
			ev->numArg = 0.0f;
			ev->sender = _actor;
			SKSE::GetModCallbackEventSource()->SendEvent(ev);
			ev = new SKSE::ModCallbackEvent();
			ev->eventName = RE::BSFixedString("NPCsDrinkPotionEvent");
			ev->strArg = RE::BSFixedString("");
			ev->numArg = 0.0f;
			ev->sender = std::get<2>(ls.front());
			SKSE::GetModCallbackEventSource()->SendEvent(ev);
		} else {
			RE::ExtraDataList* extra = new RE::ExtraDataList();
			RE::ActorEquipManager::GetSingleton()->EquipObject(_actor, std::get<2>(ls.front()), extra);
		}
		return { std::get<1>(ls.front()), std::get<3>(ls.front()) };
	}
	return { -1, AlchemyEffect::kNone };
}

std::pair<int, AlchemyEffect> ACM::ActorUsePoison(RE::Actor* _actor, uint64_t alchemyEffect)
{
	auto begin = std::chrono::steady_clock::now();
	// if no effect is specified, return
	if (alchemyEffect== 0) {
		return { -1, AlchemyEffect::kNone };
	}
	auto itemmap = _actor->GetInventory();
	auto iter = itemmap.begin();
	auto end = itemmap.end();
	//RE::EffectSetting* sett = nullptr;
	LOG_2("[ActorUsePoison] trying to find poison");
	auto ls = GetMatchingPoisons(_actor, alchemyEffect);
	ls.sort(Utility::SortMagnitude);
	// got all potions the actor has sorted by magnitude.
	// now use the one with the highest magnitude;
	if (ls.size() > 0) {
		LOG_2("[ActorUsePoison] Use Poison");
		if (Settings::_CompatibilityMode) {
			LOG_3("[ActorUsePoison] Compatibility Mode");
			// use same events as for potions, since it takes a TESForm* and works for anything
			SKSE::ModCallbackEvent* ev = new SKSE::ModCallbackEvent();
			ev->eventName = RE::BSFixedString("NPCsDrinkPotionActorInfo");
			ev->strArg = RE::BSFixedString("");
			ev->numArg = 0.0f;
			ev->sender = _actor;
			SKSE::GetModCallbackEventSource()->SendEvent(ev);
			ev = new SKSE::ModCallbackEvent();
			ev->eventName = RE::BSFixedString("NPCsDrinkPotionEvent");
			ev->strArg = RE::BSFixedString("");
			ev->numArg = 0.0f;
			ev->sender = std::get<2>(ls.front());
			SKSE::GetModCallbackEventSource()->SendEvent(ev);
		} else {
			RE::ExtraDataList* extra = new RE::ExtraDataList();
			RE::ActorEquipManager::GetSingleton()->EquipObject(_actor, std::get<2>(ls.front()), extra);
		}
		return { std::get<1>(ls.front()), std::get<3>(ls.front()) };
	}
	return { -1, AlchemyEffect::kNone };
}




