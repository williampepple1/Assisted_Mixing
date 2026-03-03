#include "MixRuleDatabase.h"

MixRule MixRuleDatabase::getRule(Genre genre, Instrument instrument)
{
    switch (genre)
    {
        case Genre::Rock:      return getRockRule(instrument);
        case Genre::Pop:       return getPopRule(instrument);
        case Genre::HipHop:    return getHipHopRule(instrument);
        case Genre::EDM:       return getEDMRule(instrument);
        case Genre::Jazz:      return getJazzRule(instrument);
        case Genre::RnB:       return getRnBRule(instrument);
        case Genre::Metal:     return getMetalRule(instrument);
        case Genre::Classical: return getClassicalRule(instrument);
        default:               return {};
    }
}

// Stub implementations - will be filled in the rules-engine step
MixRule MixRuleDatabase::getRockRule(Instrument) { return {}; }
MixRule MixRuleDatabase::getPopRule(Instrument) { return {}; }
MixRule MixRuleDatabase::getHipHopRule(Instrument) { return {}; }
MixRule MixRuleDatabase::getEDMRule(Instrument) { return {}; }
MixRule MixRuleDatabase::getJazzRule(Instrument) { return {}; }
MixRule MixRuleDatabase::getRnBRule(Instrument) { return {}; }
MixRule MixRuleDatabase::getMetalRule(Instrument) { return {}; }
MixRule MixRuleDatabase::getClassicalRule(Instrument) { return {}; }
