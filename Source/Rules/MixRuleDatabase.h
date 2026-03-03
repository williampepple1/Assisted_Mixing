#pragma once

#include "GenreInstrumentDefs.h"
#include "MixRule.h"

class MixRuleDatabase
{
public:
    static MixRule getRule(Genre genre, Instrument instrument);

private:
    static MixRule getRockRule(Instrument inst);
    static MixRule getPopRule(Instrument inst);
    static MixRule getHipHopRule(Instrument inst);
    static MixRule getEDMRule(Instrument inst);
    static MixRule getJazzRule(Instrument inst);
    static MixRule getRnBRule(Instrument inst);
    static MixRule getMetalRule(Instrument inst);
    static MixRule getClassicalRule(Instrument inst);
};
